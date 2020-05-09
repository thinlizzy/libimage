package(default_visibility = ["//FreeImage:__subpackages__"])

cc_library(
	name = "OpenEXR",
	srcs = glob(
		include = [
			"Half/*.cpp",
			"Iex/*.cpp",
			"IexMath/*.cpp",
			"IlmImf/*.cpp",
			"IlmThread/*.cpp",
			"Imath/*.cpp",
		],
		exclude = [
			"Half/eLut.cpp",
			"Half/toFloat.cpp",
			"IlmImf/dwaLookups.cpp",
			"IlmImf/b44ExpLogTable.cpp",
			"IlmThread/*Win32.cpp",
			"IlmThread/*Posix.cpp",
		]) + select({
			"@bazel_tools//src/conditions:windows": glob(["IlmThread/*Win32.cpp"]),
			"//conditions:default": glob(["IlmThread/*Posix.cpp"]),
		}),
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
	copts = select({
		"@bazel_tools//src/conditions:windows": ["-std:c++14"],
		"//conditions:default": ["-std=c++14"],
	}),
)
