cmake_minimum_required(VERSION 3.10)
project(libimage)

set(CMAKE_CXX_STANDARD 17)

add_compile_definitions(FREEIMAGE_LIB)

include_directories(image)
include_directories(FreeImage)

add_subdirectory(FreeImage)

add_library(libimage image/image.cpp image/image.h)
target_link_libraries(libimage FreeImageLib)

add_subdirectory(applications/libimagetest)
add_subdirectory(applications/extractor)

