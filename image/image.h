#ifndef IMAGE_WRAPPER_H_GUARD_KJASIDc0ewir32j42nrjfdszf93
#define IMAGE_WRAPPER_H_GUARD_KJASIDc0ewir32j42nrjfdszf93

#include <istream>
#include <memory>
#include <ostream>
#include <string>

struct FIBITMAP;

namespace img {

typedef unsigned Size;

struct Color {
    unsigned char r,g,b,a;
};

enum Type { BMP, GIF, JPG, PNG, };

enum ResizeFilter {
    box, bilinear, bspline, bicubic, catmullrom, lanczos3,
};

class Image {
	struct Deleter {
		void operator()(FIBITMAP * image) const;
	};

	std::unique_ptr<FIBITMAP,Deleter> image;
	int type;

	Image(FIBITMAP * image, int type);

	void save(std::ostream & stream, int type) const;
public:
	Image();
	Image(Image const &) = delete;
	Image & operator=(Image const &) = delete;
	Image(Image &&) = default;
	Image & operator=(Image &&) = default;
    
	explicit Image(char const * filename);
	explicit Image(std::string const & filename);
	Image(std::istream & stream, Type type);
	// ~Image();
	void load(char const * filename);
	void load(std::istream & stream, Type type);
    
    Image clone() const;

	Image thumbnail(Size squareSize) const;
    Image resize(Size width, Size height, ResizeFilter filter = bicubic) const;
    Image rotate(double degrees) const;    
    Image flipH() const;
    Image flipV() const;

	Size width() const;
	Size height() const;
	unsigned bpp() const;
	unsigned char * rawBits() const;

	std::unique_ptr<unsigned char[]> toRawBits(unsigned targetBpp) const;
    
    // slow pixel access functions. use with care. y starts at the bottom!
    int getColorIndex(Size x, Size y) const;
    Color getColor(Size x, Size y) const;
    bool isTransparentPixel(Size x, Size y) const;
    
    bool transparent() const;
    int getTransparentColorIndex() const;
    Color getColorFromIndex(int colorIndex) const;

	void * getWindowSystemHeader() const;

	void save(char const * filename) const;
	void save(std::string const & filename) const;
	void save(std::ostream & stream) const;
	void save(std::ostream & stream, Type type) const;
};

}

#endif
