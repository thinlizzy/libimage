package(default_visibility = ["//FreeImage:__subpackages__"])

cc_library(
	name = "LibRawLite",
	srcs = glob(
		include = [
			"src/*.cpp",
			"internal/*.h",
			"internal/*.c",
			"internal/*.cpp",
		],
		exclude = [
			"src/libraw_fuji_compressed.cpp",
			"src/libraw_xtrans_compressed.cpp",
			"internal/aahd_demosaic.cpp",
			"internal/dht_demosaic.cpp",
			"internal/dcb_demosaicing.c",
			"internal/wf_filtering.cpp",
		],
	) + ["dcraw/dcraw.c"],
	hdrs = glob(["libraw/*.h"]),
	textual_hdrs = [
		"src/libraw_fuji_compressed.cpp",
	],
	deps = [
		"//FreeImage/LibJPEG",
	],
	includes = [
		".",
		"src",
		"../LibJPEG",
	],
	defines = [
		"NO_JASPER",
		"NO_LCMS",
		"LIBRAW_NODLL",
	] + select({
		"@bazel_tools//src/conditions:windows": ["WIN32"],
		"//conditions:default": [],
	}),
	copts = select({
		"@bazel_tools//src/conditions:windows": ["-std:c++14"],
		"//conditions:default": ["-std=c++14"],
	}),
)
