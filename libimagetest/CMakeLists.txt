cmake_minimum_required(VERSION 3.3)
project(libimagetest)

set(SOURCE_FILES
        libimagetest.cpp)

add_executable(libimagetest ${SOURCE_FILES})
set_property(TARGET libimagetest PROPERTY CXX_STANDARD 14)

target_link_libraries(libimagetest libimage ${FREEIMAGE_LIBRARIES})