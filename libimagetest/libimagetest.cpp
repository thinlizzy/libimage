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
	std::string path = argc >=2 ? argv[1] : "";

	std::string filename;
	std::cout << "enter source filename: ";
	std::getline(std::cin,filename);
	img::Image image((path+filename).c_str());

	img::Image thumb(image.thumbnail(200));

	std::string filenameT;
	std::cout << "enter target filename: ";
	std::getline(std::cin,filenameT);
	image.save((path+filenameT).c_str());

	thumb.save((path+"thumb_"+filenameT).c_str());

	std::ifstream ifs((path+filename).c_str(),std::ios::binary);
	img::Image image2(ifs,getType(filename));

	std::ofstream ofs((path+"stream_"+filename).c_str(),std::ios::binary);
	image2.save(ofs);

	std::ofstream ofs2((path+"stream_"+filenameT).c_str(),std::ios::binary);
	image2.save(ofs,getType(filenameT));

	return 0;
}

