include_directories(.)
file(GLOB_RECURSE SRC_FILES *.h *.c)
add_library(FreeImageWebP ${SRC_FILES})
