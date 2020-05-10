package(default_visibility = ["//FreeImage:__subpackages__"])

TEXTUAL_HEADERS = [
	"src/libraw_fuji_compressed.cpp",
	"src/libraw_xtrans_compressed.cpp",
	"internal/aahd_demosaic.cpp",
	"internal/dht_demosaic.cpp",
	"internal/dcb_demosaicing.c",
	"internal/libraw_x3f.cpp",
	"internal/wf_filtering.cpp",
]

cc_library(
	name = "LibRawLite",
	srcs = glob(
		include = [
			"src/*.cpp",
			"internal/*.h",
			"internal/*.c",
			"internal/*.cpp",
		],
		exclude = TEXTUAL_HEADERS,
	) + ["dcraw/dcraw.c"],
	hdrs = glob(["libraw/*.h"]),
	textual_hdrs = TEXTUAL_HEADERS,
	deps = [
		"//FreeImage/LibJPEG",
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
	copts = select({
		"@bazel_tools//src/conditions:windows": ["-std:c++14"],
		"//conditions:default": ["-std=c++14"],
	}),
)
