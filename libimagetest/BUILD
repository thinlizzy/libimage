cc_library(
	name = "libimagetest",
	srcs = ["libimagetest.cpp"],
	deps = [
		"//libimage",
	],
	copts = [
		"--std=c++1y",
	],
)

cc_binary(
	name = "libimagetest-win32",
	deps = [
		":libimagetest",
		"@win32_user_libs//:freeimage",
	],
	copts = [
		"--std=c++1y",
	],
	linkopts = [
		"-lgdi32",
	],
)

cc_binary(
	name = "libimagetest-linux",
	deps = [
		":libimagetest",
		"@linux_libs//:freeimage",
	],
	copts = [
		"--std=c++1y",
	],
)
