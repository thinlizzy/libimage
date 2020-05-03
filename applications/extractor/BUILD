cc_library(
	name = "imageExtractor-lib",
	srcs = ["imageExtractor.cpp"],
	deps = [
		"//image",
	],
	copts = ["-std:c++17"],
)

cc_binary(
	name = "extractor",
	deps = [
		":imageExtractor-lib",
	],
)
