# linux

new_local_repository(
	name = "linux_freeimage",
	path = "/usr/lib",
	build_file_content = """
cc_library(
	name = "freeimage",
	srcs = ["libfreeimage.a"],
	visibility = ["//visibility:public"],
)
""",
)

# win32

new_local_repository(
	name = "win32_freeimage",
	path = "/d/diego/progs/c++/FreeImage/x64",
	build_file_content = """
cc_library(
	name = "freeimage",
	srcs = ["FreeImage.lib"],
	hdrs = ["include/FreeImage.h"],
	strip_include_prefix = "include",
	visibility = ["//visibility:public"],
)
""",
)
