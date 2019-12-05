include_directories(image/sys)
include_directories(jxrgluelib)

file(GLOB_RECURSE SRC_FILES *.h *.c)
add_library(FreeImageJXR ${SRC_FILES})
