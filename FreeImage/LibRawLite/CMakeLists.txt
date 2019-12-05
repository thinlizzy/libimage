include_directories(.)
include_directories(../LibJPEG)

add_compile_definitions(NO_JASPER)
add_compile_definitions(NO_LCMS)

set_source_files_properties(
        src/libraw_fuji_compressed.cpp
        src/libraw_xtrans_compressed.cpp
        internal/aahd_demosaic.cpp
        internal/dht_demosaic.cpp
        internal/dcb_demosaicing.c
        internal/wf_filtering.cpp
        PROPERTIES
        HEADER_FILE_ONLY true)

file(GLOB SRC_FILES libraw/*.h src/*.cpp internal/*.h internal/*.c internal/*.cpp)
add_library(FreeImageRawLite
        ${SRC_FILES}
        dcraw/dcraw.c
        )
