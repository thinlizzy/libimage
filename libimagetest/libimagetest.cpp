#include "../src/image.h"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>

bool lequals(std::string const & filename, std::string const & ext)
{
	if( ext.size() > filename.size() ) return false;

	using namespace std;
	return equal(ext.rbegin(),ext.rend(),filename.rbegin(),[](char a, char b){
		return tolower(a) == tolower(b);
	});
}

img::Type getType(std::string const & filename)
{
	if( lequals(filename,".bmp") ) return img::BMP;
	if( lequals(filename,".gif") ) return img::GIF;
	if( lequals(filename,".jpg") ) return img::JPG;
	if( lequals(filename,".png") ) return img::PNG;

	throw false;
}

int main(int argc, char * argv[])
{
	std::string pathS,filenameS;
	std::cout << "enter source path (with the trailing slash): " << std::flush;
	std::getline(std::cin,pathS);
	std::cout << "enter source filename: " << std::flush;
	std::getline(std::cin, filenameS);

	std::cout << "loading image" << std::endl;
	img::Image image(pathS+filenameS);
	std::cout << "creating thumbnail" << std::endl;
	img::Image thumb(image.thumbnail(200));

	std::string pathT,filenameT;
	std::cout << "enter target path (with the trailing slash): " << std::flush;
	std::getline(std::cin,pathT);
	std::cout << "enter target filename: " << std::flush;
	std::getline(std::cin,filenameT);

	std::cout << "saving new file" << std::endl;
	image.save(pathT+filenameT);
	std::cout << "saving a thumbnail" << std::endl;
	thumb.save(pathT+"thumb_"+filenameT);

	std::cout << "opening source file as a stream" << std::endl;
	std::ifstream ifs(pathS+filenameS,std::ios::binary);
	if( ! ifs ) throw "error opening source file as a stream";
	img::Image image2(ifs,getType(pathS+filenameS));

	std::cout << "saving source file as a stream" << std::endl;
	std::ofstream ofs(pathT+"stream_"+filenameS,std::ios::binary|std::ios::out);
	if( ! ofs ) throw "error opening first target file as a stream";
	image2.save(ofs);

	std::cout << "saving target file as a stream" << std::endl;
	std::ofstream ofs2(pathT+"stream_"+filenameT,std::ios::binary|std::ios::out);
	if( ! ofs2 ) throw "error opening second target file as a stream";
	image2.save(ofs2,getType(filenameT));

	return 0;
}
