package(default_visibility = ["//visibility:public"])

cc_library(
	name = "libimage",
	srcs = ["src/image.cpp"],
	hdrs = ["src/image.h"],
	deps = ["//external:freeimage"],
	copts = ["--std=c++1y"],
	linkstatic = 1,
)
