cc_library(
	name = "imageExtractor-lib",
	srcs = ["imageExtractor.cpp"],
	deps = [
		"//image",
	],
)

cc_binary(
	name = "extractor",
	deps = [
		":imageExtractor-lib",
	],
)
