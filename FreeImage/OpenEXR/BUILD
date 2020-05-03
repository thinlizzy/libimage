package(default_visibility = ["//FreeImage:__subpackages__"])

cc_library(
	name = "OpenEXR",
	srcs = glob(
		include = ["**/*.cpp"],
		exclude = [
			"Half/eLut.cpp",
			"Half/toFloat.cpp",
			"IlmImf/dwaLookups.cpp",
			"IlmImf/b44ExpLogTable.cpp",
		]),
	hdrs = glob(["**/*.h"]),
	deps = [
	],
	includes = [
		".",
		"Half",
		"Iex",
		"IexMath",
		"IlmImf",
		"Imath",
		"IlmThread",
		"../ZLib",
	],
)
