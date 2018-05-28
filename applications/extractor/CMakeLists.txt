add_executable(imageextractor imageExtractor.cpp)

target_link_libraries(imageextractor libimage ${FREEIMAGE_LIBRARIES})