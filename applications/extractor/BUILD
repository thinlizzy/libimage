cc_library(
	name = "imageExtractor-lib",
	srcs = ["imageExtractor.cpp"],
	deps = [
		"//image",
	],
	copts = select({
		"@bazel_tools//src/conditions:windows": ["-std:c++17"],
		"//conditions:default": ["-std=c++17"],
	}),
)

cc_binary(
	name = "extractor",
	deps = [
		":imageExtractor-lib",
	],
)
