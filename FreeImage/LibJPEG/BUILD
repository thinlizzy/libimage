package(default_visibility = ["//FreeImage:__subpackages__"])

cc_library(
	name = "LibJPEG",
	srcs = glob(
		include = [
			"*.h",
			"*.c",
		],
		exclude = [
			"jpeglib.h",
			"cjpeg.c",
			"ckconfig.c",
			"jpegtran.c",
			"rdjpgcom.c",
			"wrjpgcom.c",
			"jmemansi.c",
			"jmemdos.c",
			"jmemmac.c",
			"jmemname.c",
			"djpeg.c",
		],
	),
	hdrs = ["jpeglib.h"],
	copts = select({
		"@bazel_tools//src/conditions:windows": ["-std:c++14"],
		"//conditions:default": ["-std=c++14"],
	}),
)
