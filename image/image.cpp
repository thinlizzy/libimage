#include "image.h"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <stdexcept>
#include <FreeImage.h>

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

Type TypeFromExtensionImpl(std::string ext) {
	if( ext.empty() ) throw std::runtime_error("filename has no extension");
	std::transform(ext.begin(),ext.end(),ext.begin(),[](char ch) { return char(std::toupper(ch)); });
	if( ext == ".BMP" ) return BMP;
	if( ext == ".GIF" ) return GIF;
	if( ext == ".JPG" ) return JPG;
	if( ext == ".PNG" ) return PNG;
	throw std::runtime_error("unsupported extension " + ext);
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

RGBQUAD toRgbQuad(Color color) {
	RGBQUAD result;
	result.rgbRed = color.r;
	result.rgbGreen = color.g;
	result.rgbBlue = color.b;
	result.rgbReserved = color.a;
	return result;
}


Image::Image():
	type(FIF_UNKNOWN)
{}

Image::Image(Size width, Size height, int bpp):
	Image(FreeImage_Allocate(width,height,bpp),FIF_UNKNOWN)
{
	if( ! image ) {
		throw std::runtime_error("failed to create image");
	}
}

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

Image Image::to32bpp() const
{
	FIBITMAP * cloneDib = FreeImage_ConvertTo32Bits(image.get());
	if( cloneDib == 0 ) throw std::runtime_error("error converting image to 32bpp");

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

Image Image::clip(int left, int top, int right, int bottom) const
{
	FIBITMAP * cloneDib = FreeImage_Copy(image.get(),left,top,right+1,bottom+1);
	if( cloneDib == 0 ) throw std::runtime_error("error clipping image");
	return Image(cloneDib,type);
}


Image & Image::replace(Color origColor, Color newColor) {
	auto origQuad = toRgbQuad(origColor);
	auto newQuad = toRgbQuad(newColor);
	FreeImage_ApplyColorMapping(image.get(),&origQuad,&newQuad,1,false,false);
	return *this;
}

Image &	Image::replaceColors(std::function<bool(img::Color)> predicate, std::function<img::Color(img::Color)> colorChanger) {
	for( Size r = 0; r != height(); ++r )
		for( Size c = 0; c != width(); ++c ) {
			auto color = getColor(c,r);
			if( predicate(color) ) {
				setColor(c,r,colorChanger(color));
			}
		}
	return *this;
}

bool Image::pasteFrom(const Image & subImage, Size x, Size y) {
	if( ! FreeImage_Paste(image.get(),subImage.image.get(),x,y,256) ) { // >255 = no alpha blend
		return false;
	}
	return true;
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
	FreeImage_GetPixelIndex(image.get(),x,height()-y-1,&result);
	return result;
}

Color Image::getColor(Size x, Size y) const {
	RGBQUAD value;
	// todo log if false
	FreeImage_GetPixelColor(image.get(),x,height()-y-1,&value);
	return {value.rgbRed,value.rgbGreen,value.rgbBlue,value.rgbReserved};
}

Image & Image::setColor(Size x, Size y, Color color) {
	auto quad = toRgbQuad(color);
	FreeImage_SetPixelColor(image.get(),x,height()-y-1,&quad);
	return *this;
}

bool Image::isTransparentPixel(Size x, Size y) const {
	auto table = FreeImage_GetTransparencyTable(image.get());
	assert(table != nullptr);
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

void Image::save(char const * filename) const {
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

void Image::save(std::string const & filename) const
{
	save(filename.c_str());
}

void Image::save(wchar_t const * filename) const {
	if( ! image ) throw std::runtime_error("can't save empty image");

	// try to guess the file format from the file extension
	FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilenameU(filename);
	if( fif == FIF_UNKNOWN ) throw std::runtime_error("image filename not supported");
	if( ! FreeImage_FIFSupportsWriting(fif) ) throw std::runtime_error("image file format not supported");

	auto bpp = FreeImage_GetBPP(image.get());
	if( ! FreeImage_FIFSupportsExportBPP(fif, bpp) ) throw std::runtime_error("image file format not supported for current bpp");

	// flag = 0 for now. need to customize it in some way
	if( ! FreeImage_SaveU(fif, image.get(), filename, 0 ) ) throw std::runtime_error("error saving image");
}

void Image::save(std::wstring const & filename) const {
	save(filename.c_str());
}

unsigned DLL_CALLCONV ReadProc(void * buffer, unsigned size, unsigned count, fi_handle handle)
{
	auto & stream = *reinterpret_cast<std::istream *>(handle);
	auto buf = static_cast<char *>(buffer);
	std::streamsize bytesRead = 0;

	// try to read as much as possible when size == 1 , which is a very common case
	if( size == 1 ) {
		while( stream && bytesRead < count ) {
			stream.read(buf,count - bytesRead);
			auto tot = stream.gcount();
			if( tot == 0 ) break;
			bytesRead += tot;
			buf += tot;
		}
		return bytesRead;
	}

	unsigned result = 0;
	auto chunk = std::streamsize(size);
	while( stream ) {
		stream.read(buf,chunk);
		auto tot = stream.gcount();
		if( tot == 0 ) break;
		bytesRead += tot;
		buf += tot;
		while( bytesRead >= chunk ) {
			++result;
			if( result >= count ) {
				return result;
			}
			bytesRead -= chunk;
		}
	}
	if( bytesRead > 0 ) ++result;
	return result;
}

// TODO implement special case when size == 1
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
	if( type == FIF_UNKNOWN ) {
		throw std::runtime_error("can't save images with unspecified format to a stream");
	}
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

Image::operator bool() const {
	return bool(image);
}

ImageInitializer::ImageInitializer() {
	class ImageInitializerHelper {
	public:
		ImageInitializerHelper() {
			FreeImage_Initialise();
		}
		~ImageInitializerHelper() {
			FreeImage_DeInitialise();
		}
	};
	static ImageInitializerHelper init;
}

Type TypeFromExtension(char const * filename) {
	auto pt = std::strrchr(filename,'.');
	if( ! pt ) throw std::runtime_error("filename has no extension");
	return TypeFromExtensionImpl(std::string(pt));
}

Type TypeFromExtension(std::string const & filename) {
	auto p = filename.rfind('.');
	return TypeFromExtensionImpl(filename.substr(p));
}

}
