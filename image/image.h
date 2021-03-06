#ifndef IMAGE_WRAPPER_H_GUARD_KJASIDc0ewir32j42nrjfdszf93
#define IMAGE_WRAPPER_H_GUARD_KJASIDc0ewir32j42nrjfdszf93

#include <functional>
#include <istream>
#include <memory>
#include <ostream>
#include <string>

struct FIBITMAP;

namespace img {

typedef unsigned Size;

struct Color {
	using component = unsigned char;
	component r,g,b,a;
	bool operator==(Color const & color) const {
		return r == color.r && g == color.g && b == color.b && a == color.a;
	}
	bool operator!=(Color const & color) const {
		return ! operator==(color);
	}
};

enum Type { BMP, GIF, JPG, PNG, };

enum ResizeFilter {
	box, bilinear, bspline, bicubic, catmullrom, lanczos3,
};

class ImageInitializer {
protected:
	ImageInitializer();
};

class Image: private ImageInitializer {
	struct Deleter {
		void operator()(FIBITMAP * image) const;
	};

	std::unique_ptr<FIBITMAP,Deleter> image;
	int type;

	Image(FIBITMAP * image, int type);

	void save(std::ostream & stream, int type) const;
public:
	Image(); // create a zombie image
	// ~Image();
	Image(Image const &) = delete;
	Image & operator=(Image const &) = delete;
	Image(Image &&) = default;
	Image & operator=(Image &&) = default;

 	/** Create a black image. */
	Image(Size width, Size height, int bpp);
	explicit Image(char const * filename);
	explicit Image(std::string const & filename);
	Image(std::istream & stream, Type type);

	void load(char const * filename);
	void load(std::istream & stream, Type type);

	Image clone() const;
	Image to32bpp() const;

	Image thumbnail(Size squareSize) const;
	Image resize(Size width, Size height, ResizeFilter filter = bicubic) const;
	Image rotate(double degrees) const;
	Image flipH() const;
	Image flipV() const;
	// rectangle is closed
	Image clip(int left, int top, int right, int bottom) const;

	// non-const functions
	Image & replace(Color origColor, Color newColor);
	Image & replaceColors(std::function<bool(img::Color)> predicate, std::function<img::Color(img::Color)> colorChanger);

	bool pasteFrom(Image const & subImage, Size x, Size y);

	Size width() const;
	Size height() const;
	unsigned bpp() const;
	unsigned char * rawBits() const;

	std::unique_ptr<unsigned char[]> toRawBits(unsigned targetBpp) const;

	// slow pixel access functions. use with care. y starts at the bottom!
	int getColorIndex(Size x, Size y) const;
	Color getColor(Size x, Size y) const;
	bool isTransparentPixel(Size x, Size y) const;
	Image & setColor(Size x, Size y, Color color);

	bool transparent() const;
	int getTransparentColorIndex() const;
	Color getColorFromIndex(int colorIndex) const;

	void * getWindowSystemHeader() const;

	void save(char const * filename) const;
	void save(std::string const & filename) const;
	void save(wchar_t const * filename) const;
	void save(std::wstring const & filename) const;
	void save(std::ostream & stream) const;
	void save(std::ostream & stream, Type type) const;

	explicit operator bool() const;
};

Type TypeFromExtension(char const * filename);
Type TypeFromExtension(std::string const & filename);

inline std::ostream & operator<<(std::ostream & os, Color const & color) {
	return os << '(' << int(color.r) << ',' << int(color.g) << ',' << int(color.b) << ',' << int(color.a) << ')';
}

inline std::istream & operator>>(std::istream & is, Color & color) {
	auto readCharAsInt = [&is](Color::component & cp) {
		int c;
		is >> c;
		cp = c;
	};
	auto skipOrFail = [&is](char c) {
		if( is.peek() != c ) {
			is.setstate(std::ios_base::failbit);
			return false;
		}
		is.get();
		return true;
	};
	// TODO only ignore control chars until reaching '('
	is.ignore(std::numeric_limits<std::streamsize>::max(),'(');
	readCharAsInt(color.r);
	if( ! skipOrFail(',') ) return is;
	readCharAsInt(color.g);
	if( ! skipOrFail(',') ) return is;
	readCharAsInt(color.b);
	if( ! skipOrFail(',') ) return is;
	readCharAsInt(color.a);
	skipOrFail(')');
	return is;
}

}

#endif
