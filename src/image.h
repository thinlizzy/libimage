#ifndef IMAGE_WRAPPER_H_GUARD_KJASIDc0ewir32j42nrjfdszf93
#define IMAGE_WRAPPER_H_GUARD_KJASIDc0ewir32j42nrjfdszf93

#include <memory>
#include <istream>
#include <ostream>
#include <string>

struct FIBITMAP;

namespace img {

typedef unsigned Size;

enum Type { BMP, GIF, JPG, PNG, };

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
	Image(Image &&) = default;
	//Image & operator=(Image &&) = default;
	explicit Image(char const * filename);
	explicit Image(std::string const & filename);
	Image(std::istream & stream, Type type);
	// ~Image();
	void load(char const * filename);
	void load(std::istream & stream, Type type);

	Size width() const;
	Size height() const;
	unsigned bpp() const;
	unsigned char * rawBits() const;

	void * getWindowSystemHeader() const;

	Image thumbnail(Size squareSize) const;
	void save(char const * filename) const;
	void save(std::ostream & stream) const;
	void save(std::ostream & stream, Type type) const;
};

}

#endif
