cc_library(
	name = "libimagetest-lib",
	srcs = ["libimagetest.cpp"],
	deps = [
		"//:libimage",
	],
	copts = [
		"--std=c++1y",
	],
)

cc_binary(
	name = "libimagetest",
	deps = [
		":libimagetest-lib",
		"//external:freeimage",
	],
	copts = [
		"--std=c++1y",
	],
)
