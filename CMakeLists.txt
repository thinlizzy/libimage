cmake_minimum_required(VERSION 3.10)
project(libimage)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(FREEIMAGE_FIND_REQUIRED TRUE)
find_package(FreeImage)
include_directories(SYSTEM ${FREEIMAGE_INCLUDE_DIRS})

include_directories(image)

add_subdirectory("image")
add_subdirectory("applications/libimagetest")
add_subdirectory("applications/extractor")
