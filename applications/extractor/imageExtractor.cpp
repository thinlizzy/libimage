#include "image.h"

#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std::string_literals;

template<typename T>
void die(T msg) {
    std::cerr << msg << '\n';
    std::exit(1);
}

struct CommandLine {
    std::vector<std::string> parameters;
    std::unordered_map<std::string, std::string> flags;

    std::string flagOrDie(std::string const & flagname) {
        auto it = flags.find(flagname);
        if( it == flags.end() ) die("missing " + flagname + " flag");
        return it->second;
    }
    std::string flagOrDefault(std::string const & flagname, std::string const & defValue) {
        auto it = flags.find(flagname);
        return it == flags.end() ? defValue : it->second;
    }
};

struct Point {
    int x,y;

    static Point parse(std::string const & str) {
        auto iss = std::istringstream(str);
        auto result = Point();
        iss >> result.x;
        if( iss.get() != 'x' ) die("missing x when parsing point");
        iss >> result.y;
        if( result.x < 0 || result.y < 0 ) die("invalid negative values in point");
        return result;
    }
};

struct FilenameBuilder {
    std::string filename;
    std::string extension;
    int digits;

    explicit FilenameBuilder(std::string const & fileAndExt, int maxNum) {
        auto iPt = fileAndExt.rfind('.');
        if( iPt == std::string::npos ) die("missing extension in the filename");
        filename = fileAndExt.substr(0,iPt);
        extension = fileAndExt.substr(iPt);
        digits = int(std::log10(maxNum)) + 1;
    }

    std::string build(int num) {
        std::ostringstream oss;
        oss << filename << std::setfill('0') << std::setw(digits) << num << extension;
        return oss.str();
    }
};

CommandLine parseCommandLine(int argc, char **argv);

// TODO add a flag to save all images in a single one
int main(int argc, char * argv[]) {
    auto commandLine = parseCommandLine(argc, argv);
    if( commandLine.parameters.size() < 2 ) die("no source and target files specified");
    auto targetImages = std::stoi(commandLine.flagOrDefault("target_images","1"));
    if( targetImages < 1 ) die("invalid target_images flag");
    auto targetImageNameBuilder = FilenameBuilder(commandLine.parameters[1],targetImages);
    auto startingPoint = Point::parse(commandLine.flagOrDefault("starting_point","0x0"));
    auto tgtImgSize = Point::parse(commandLine.flagOrDie("target_image_size"));
    auto endingPoint = Point{
        tgtImgSize.x + startingPoint.x,
        tgtImgSize.y + startingPoint.y,
    };
    auto offsetX = std::stoi(commandLine.flagOrDefault("offset_x","0"));
    offsetX += tgtImgSize.x;
    auto offsetY = std::stoi(commandLine.flagOrDefault("offset_y","0"));
    offsetY += tgtImgSize.y;
    auto imagesPerLine = std::stoi(commandLine.flagOrDefault("images_per_line","-1"));
    if( imagesPerLine < -1 ) die("invalid images_per_line flag");
    if( imagesPerLine == -1 ) imagesPerLine = std::numeric_limits<int>::max();

    auto sourceImage = img::Image(commandLine.parameters[0]);

    auto topLeft = startingPoint;
    auto bottomRight = endingPoint;
    for( int i = 1; i <= targetImages; ++i ) {
        auto targetImage = sourceImage.clip(topLeft.x,topLeft.y,bottomRight.x,bottomRight.y);
        auto targetFilename = targetImageNameBuilder.build(i);
        std::cout << "writing " << targetFilename << std::endl;
        targetImage.save(targetFilename);
        if( i % imagesPerLine == 0 ) {
            topLeft.x = startingPoint.x;
            topLeft.y += offsetY;
            bottomRight.x = endingPoint.x;
            bottomRight.y += offsetY;
        } else {
            topLeft.x += offsetX;
            bottomRight.x += offsetX;
        }
    }
}

CommandLine parseCommandLine(int argc, char **argv) {
    auto result = CommandLine();
    auto flagname = std::string();
    for (int i = 1; i < argc; ++i) {
        auto current = std::string(argv[i]);
        if( flagname.empty() ) {
            if (current[0] == '-') {
                flagname = std::move(current);
                flagname = flagname.substr(1);
            } else {
                result.parameters.push_back(std::move(current));
            }
        } else {
            result.flags.insert_or_assign(std::move(flagname),std::move(current));
            flagname.clear(); // it is moved away, but...
        }
    }
    return result;
}
