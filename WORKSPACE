# TODO add dependency to FreeImage.h somehow

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
	visibility = ["//visibility:public"],
)
""",
)

# bindings for multiplatform

# if linux then

bind(
    name = "freeimage",
    actual = "@linux_freeimage//:freeimage",
)

# else if win32 then

# bind(
#     name = "freeimage",
#     actual = "@win32_freeimage//:freeimage",
# )

# end if
