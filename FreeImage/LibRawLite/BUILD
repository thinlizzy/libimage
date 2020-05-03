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
	deps = [
	],
	includes = [
		".",
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
	copts = ["-std:c++14"],
)
