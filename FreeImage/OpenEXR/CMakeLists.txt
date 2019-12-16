include_directories(.)
include_directories(Half)
include_directories(Iex)
include_directories(IexMath)
include_directories(IlmImf)
include_directories(Imath)
include_directories(IlmThread)
include_directories(../ZLib)

file(GLOB_RECURSE SRC_FILES *.h *.cpp)
file(GLOB SRC_REMOVE
        Half/eLut.cpp
        Half/toFloat.cpp
        IlmImf/dwaLookups.cpp
        IlmImf/b44ExpLogTable.cpp)
list(REMOVE_ITEM SRC_FILES ${SRC_REMOVE})
add_library(FreeImageOpenEXR ${SRC_FILES})
