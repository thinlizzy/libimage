cmake_minimum_required(VERSION 3.3)
project(libimage)

set(SOURCE_FILES
    src/image.cpp
    src/image.h)

add_library(libimage ${SOURCE_FILES})
set_property(TARGET libimage PROPERTY CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(FREEIMAGE_FIND_REQUIRED, TRUE)
find_package(FreeImage)
include_directories(SYSTEM ${FREEIMAGE_INCLUDE_DIRS})
target_link_libraries(libimage ${FREEIMAGE_LIBRARIES})