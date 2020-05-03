package(default_visibility = ["//FreeImage:__subpackages__"])

cc_library(
	name = "LibJXR",
	srcs = glob(["**/*.c"]),
	hdrs = glob(["**/*.h"]),
	deps = [
	],
	includes = [
		"image/sys",
		"jxrgluelib",
	],
	defines = select({
		"@bazel_tools//src/conditions:windows": ["WIN32"],
		"//conditions:default": [],
	}),
)
