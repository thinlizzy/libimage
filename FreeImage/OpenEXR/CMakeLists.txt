include_directories(.)
include_directories(Half)
include_directories(Iex)
include_directories(IexMath)
include_directories(IlmImf)
include_directories(Imath)
include_directories(IlmThread)
include_directories(../ZLib)

file(GLOB_RECURSE SRC_FILES *.h *.cpp)
list(REMOVE_ITEM SRC_FILES
        Half/eLut.cpp
        Half/toFloat.cpp
        IlmImf/dwaLookups.cpp
        IlmImf/b44ExpLogTable.cpp)
add_library(FreeImageOpenEXR ${SRC_FILES})
