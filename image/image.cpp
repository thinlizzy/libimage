#include "image.h"
#include <FreeImage.h>
#include <stdexcept>
#include <cassert>

namespace img {

FREE_IMAGE_FORMAT type2fif(Type type)
{
	switch(type) {
        case BMP: return FIF_BMP;
        case GIF: return FIF_GIF;
        case JPG: return FIF_JPEG;
        case PNG: return FIF_PNG;
        default: return FIF_UNKNOWN;
	}
}

FREE_IMAGE_FILTER convertFilter(ResizeFilter filter)
{
    switch(filter) {
        case box: return FILTER_BOX;
        case bilinear: return FILTER_BILINEAR;
        case bspline: return FILTER_BSPLINE;
        case bicubic: return FILTER_BICUBIC;
        case catmullrom: return FILTER_CATMULLROM;
        case lanczos3: return FILTER_LANCZOS3;
        default: return FILTER_BOX;
    }
}


Image::Image():
	type(FIF_UNKNOWN)
{}

Image::Image(char const * filename)
{
	load(filename);
}

Image::Image(std::string const & filename)
{
	load(filename.c_str());
}

Image::Image(std::istream & stream, Type type)
{
	load(stream,type);
}

Image::Image(FIBITMAP * image, int type):
	image(image),
	type(type)
{
}

// Image::~Image() {}

void Image::Deleter::operator()(FIBITMAP * image) const
{
	FreeImage_Unload(image);
}

void Image::load(char const * filename)
{
	// check the file signature and deduce its format
	// (the second argument is currently not used by FreeImage)
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename, 0);
	if( fif == FIF_UNKNOWN ) {
		// no signature ?
		// try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(filename);
		if( fif == FIF_UNKNOWN ) throw std::runtime_error("can't autodetect image format");
	}

	// check that the plugin has reading capabilities ...
	if( ! FreeImage_FIFSupportsReading(fif) ) throw std::runtime_error("plugin can't load image");

	// ok, let's load the file
	FIBITMAP * dib = FreeImage_Load(fif, filename, 0);		// flag = 0 for now. need to customize it in some way
	if( dib == 0 ) throw std::runtime_error("error loading image " + std::string(filename));

	// unless a bad file format, we are done !
	image.reset(dib);
	type = fif;
}

Image Image::clone() const
{
    FIBITMAP * cloneDib = FreeImage_Clone(image.get());
	if( cloneDib == 0 ) throw std::runtime_error("error cloning image");
    
	return Image(cloneDib,type);
}

Image Image::rotate(double degrees) const
{
    FIBITMAP * cloneDib = FreeImage_Rotate(image.get(),degrees);
	if( cloneDib == 0 ) throw std::runtime_error("error rotating image");
    
	return Image(cloneDib,type);
}

Image Image::flipH() const
{
    auto result = clone();
    FreeImage_FlipHorizontal(result.image.get());
    return result;
}

Image Image::flipV() const
{
    auto result = clone();
    FreeImage_FlipVertical(result.image.get());
    return result;
}


Size Image::width() const
{
	return FreeImage_GetWidth(image.get());
}

Size Image::height() const
{
	return FreeImage_GetHeight(image.get());
}

unsigned Image::bpp() const
{
	return FreeImage_GetBPP(image.get());
}

unsigned char * Image::rawBits() const
{
	return FreeImage_GetBits(image.get());
}

std::unique_ptr<unsigned char[]> Image::toRawBits(unsigned targetBpp) const
{
	auto scanWidth = width() * (targetBpp/8);
	unsigned char * result = new unsigned char[height() * scanWidth];
	FreeImage_ConvertToRawBits(result, image.get(), int(scanWidth), targetBpp,
		FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);
	return std::unique_ptr<unsigned char[]>(result);
}

void * Image::getWindowSystemHeader() const
{
	// TODO use pImpl to select between windows and linux
	return FreeImage_GetInfo(image.get());
}

bool Image::transparent() const
{
    return FreeImage_IsTransparent(image.get());
}

int Image::getTransparentColorIndex() const
{
    return FreeImage_GetTransparentIndex(image.get());
}

Color Image::getColorFromIndex(int colorIndex) const
{
	// todo log if colorIndex is out of range
	RGBQUAD * pal = FreeImage_GetPalette(image.get());
	auto & value = pal[colorIndex];
	return {value.rgbRed,value.rgbGreen,value.rgbBlue,value.rgbReserved};
}

int Image::getColorIndex(Size x, Size y) const
{
    BYTE result;
    // todo log if false
    FreeImage_GetPixelIndex(image.get(),x,y,&result);
    return result;
}

Color Image::getColor(Size x, Size y) const
{
    RGBQUAD value;
    // todo log if false
    FreeImage_GetPixelColor(image.get(),x,y,&value);
    return {value.rgbRed,value.rgbGreen,value.rgbBlue,value.rgbReserved};
}

bool Image::isTransparentPixel(Size x, Size y) const
{
	auto table = FreeImage_GetTransparencyTable(image.get());
	assert(table != NULL);
	return table[getColorIndex(x,y)] == 0;
}

Image Image::thumbnail(Size squareSize) const
{
	FIBITMAP * thumbnail = FreeImage_MakeThumbnail(image.get(), squareSize, true);
	if( ! thumbnail ) throw std::runtime_error("could not generate thumbnail");

	return Image(thumbnail,type);
}

Image Image::resize(Size width, Size height, ResizeFilter filter) const
{
	FIBITMAP * result = FreeImage_Rescale(image.get(), width, height, convertFilter(filter));
	if( ! result ) throw std::runtime_error("could not rescale image");
    
	return Image(result,type);    
}

void Image::save(char const * filename) const
{
	if( ! image ) throw std::runtime_error("can't save empty image");

	// try to guess the file format from the file extension
	FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(filename);
	if( fif == FIF_UNKNOWN ) throw std::runtime_error("image filename not supported");
	if( ! FreeImage_FIFSupportsWriting(fif) ) throw std::runtime_error("image file format not supported");

	auto bpp = FreeImage_GetBPP(image.get());
	if( ! FreeImage_FIFSupportsExportBPP(fif, bpp) ) throw std::runtime_error("image file format not supported for current bpp");

	// flag = 0 for now. need to customize it in some way
	if( ! FreeImage_Save(fif, image.get(), filename, 0 ) ) throw std::runtime_error("error saving image");
}

void Image::save(std::string const & filename) const {
	save(filename.c_str());
}


unsigned DLL_CALLCONV ReadProc(void * buffer, unsigned size, unsigned count, fi_handle handle)
{
	std::istream & stream = *reinterpret_cast<std::istream *>(handle);
	char * buf = static_cast<char *>(buffer);
	unsigned result = 0;
	while( result < count ) {
		stream.read(buf,size);
		if( stream.gcount() < std::streamsize(size) ) break;
		buf += size;
		++result;
	}
	return result;
}

unsigned DLL_CALLCONV WriteProc(void * buffer, unsigned size, unsigned count, fi_handle handle)
{
	std::ostream & stream = *reinterpret_cast<std::ostream *>(handle);
	char * buf = static_cast<char *>(buffer);
	unsigned result = 0;
	while( stream && result < count ) {
		stream.write(buf,size);
		buf += size;
		++result;
	}
	return result;
}

long DLL_CALLCONV TellProcR(fi_handle handle)
{
	std::istream & stream = *reinterpret_cast<std::istream *>(handle);
	return stream.tellg();
}

long DLL_CALLCONV TellProcW(fi_handle handle)
{
	std::ostream & stream = *reinterpret_cast<std::ostream *>(handle);
	return stream.tellp();
}

std::ios_base::seekdir getDir(int origin)
{
	auto dir = std::ios_base::beg;
	switch(origin) {
	case SEEK_CUR:
		dir = std::ios_base::cur;
		break;
	case SEEK_END:
		dir = std::ios_base::end;
		break;
	}
	return dir;
}

int DLL_CALLCONV SeekProcR(fi_handle handle, long offset, int origin)
{
	std::istream & stream = *reinterpret_cast<std::istream *>(handle);
	return stream.seekg(offset,getDir(origin)) ? 0 : -1;
}

int DLL_CALLCONV SeekProcW(fi_handle handle, long offset, int origin)
{
	std::ostream & stream = *reinterpret_cast<std::ostream *>(handle);
	return stream.seekp(offset,getDir(origin)) ? 0 : -1;
}

void Image::save(std::ostream & stream) const
{
	save(stream,type);
}

void Image::save(std::ostream & stream, Type type) const
{
	save(stream,type2fif(type));
}

void Image::save(std::ostream & stream, int type) const
{
	FreeImageIO io;
	io.read_proc  = 0;
	io.write_proc = &WriteProc;
	io.tell_proc  = &TellProcW;
	io.seek_proc  = &SeekProcW;
	// flag = 0 for now. need to customize it in some way
	if( ! FreeImage_SaveToHandle(FREE_IMAGE_FORMAT(type),image.get(),&io,reinterpret_cast<fi_handle>(&stream),0) ) throw std::runtime_error("error saving image to stream");
}

void Image::load(std::istream & stream, Type type)
{
	FreeImageIO io;
	io.read_proc  = &ReadProc;
	io.write_proc = 0;
	io.tell_proc  = &TellProcR;
	io.seek_proc  = &SeekProcR;
	// flag = 0 for now. need to customize it in some way
	auto fif = type2fif(type);
	auto dib = FreeImage_LoadFromHandle(fif,&io,reinterpret_cast<fi_handle>(&stream),0);
	if( ! dib ) throw std::runtime_error("error loading image from stream");

	image.reset(dib);
	this->type = fif;
}

}
