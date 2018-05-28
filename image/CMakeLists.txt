set(SOURCE_FILES
        image.cpp
        image.h)

add_library(libimage ${SOURCE_FILES})
target_link_libraries(libimage ${FREEIMAGE_LIBRARIES})
