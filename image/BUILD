package(default_visibility = ["//visibility:public"])

cc_library(
	name = "image",
	srcs = ["image.cpp"],
	hdrs = ["image.h"],
	deps = select({
		"@bazel_tools//src/conditions:windows": ["@win32_freeimage//:freeimage"],
		"//conditions:default": ["@linux_freeimage//:freeimage"],
	}),
)
