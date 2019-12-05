set(CMAKE_CXX_STANDARD 14)

include_directories(LibJXR/image/sys)
include_directories(LibJXR/jxrgluelib)
include_directories(OpenEXR)
include_directories(OpenEXR/Half)
include_directories(OpenEXR/Iex)
include_directories(OpenEXR/IlmImf)
include_directories(OpenEXR/Imath)
include_directories(ZLib)

add_compile_definitions(USE_JPIP)

add_compile_definitions(OPJ_STATIC)

add_compile_definitions(LIBRAW_NODLL)

file(GLOB SRC_FILES
        *.h
        FreeImage/*.h
        FreeImage/*.cpp
        FreeImageToolkit/*.h
        FreeImageToolkit/*.cpp
        LibPNG/*.h
        LibPNG/*.c
        LibJPEG/*.h
        LibJPEG/*.c
        LibOpenJPEG/*.h
        LibOpenJPEG/*.c
        Metadata/*.h
        Metadata/*.cpp
        )
file(GLOB SRC_REMOVE
        LibJPEG/cjpeg.c
        LibJPEG/ckconfig.c
        LibJPEG/jpegtran.c
        LibJPEG/rdjpgcom.c
        LibJPEG/wrjpgcom.c
        LibJPEG/jmemansi.c
        LibJPEG/jmemdos.c
        LibJPEG/jmemmac.c
        LibJPEG/jmemname.c
        LibJPEG/djpeg.c
        LibOpenJPEG/t1_generate_luts.c
        LibPNG/pngtest.c
        )
list(REMOVE_ITEM SRC_FILES ${SRC_REMOVE})
add_library(FreeImageLib ${SRC_FILES})

add_subdirectory(LibJXR)
add_subdirectory(LibRawLite)
add_subdirectory(LibTIFF4)
add_subdirectory(LibWebP)
add_subdirectory(OpenEXR)
add_subdirectory(ZLib)

target_link_libraries(FreeImageLib FreeImageOpenEXR FreeImageJXR FreeImageRawLite FreeImageZLib FreeImageWebP tiff)

