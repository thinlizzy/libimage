package(default_visibility = ["//image:__subpackages__"])

cc_library(
	name = "FreeImage",
	srcs = glob(
		include = [
			"*.h",
			"FreeImage/*.h",
			"FreeImage/*.cpp",
			"FreeImageToolkit/*.h",
			"FreeImageToolkit/*.cpp",
			"LibPNG/*.h",
			"LibPNG/*.c",
			"LibOpenJPEG/*.h",
			"LibOpenJPEG/*.c",
			"Metadata/*.h",
			"Metadata/*.cpp",
		],
		exclude = [
			"FreeImage.h",
			"LibOpenJPEG/t1_generate_luts.c",
			"LibPNG/pngtest.c",
		],
	),
	hdrs = ["FreeImage.h"],
	deps = [
		"//FreeImage/LibJPEG",
		"//FreeImage/LibJXR",
		"//FreeImage/LibRawLite",
		"//FreeImage/LibTIFF4",
		"//FreeImage/LibWebP",
		"//FreeImage/OpenEXR",
		"//FreeImage/ZLib",
	],
	defines = [
		"FREEIMAGE_LIB",
		"USE_JPIP",
		"OPJ_STATIC",
		"LIBRAW_NODLL",
	],
	includes = [
		".",
		"FreeImage",
	],
	copts = select({
		"@bazel_tools//src/conditions:windows": ["-std:c++14"],
		"//conditions:default": ["-std=c++14"],
	}),
)
