package(default_visibility = ["//visibility:public"])
cc_library(
    name = "angle_diff",
    hdrs = ["angle_diff.hh"],
    visibility = ["//visibility:public"]
)

cc_test(
    name = "angle_diff_test",
    srcs = ["angle_diff_test.cc"],
    deps = [
        ":angle_diff",
        "//external:gtest",
    ]
)


cc_library(
    name = "huber",
    hdrs = ["huber.hh"],
)

cc_library(
    name = "numerical_diff",
    hdrs = ["numerical_diff.hh"],
    srcs = ["numerical_diff.cc"],
    deps = ["//external:eigen",],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "numerical_diff_test",
    srcs = ["numerical_diff_test.cc"],
    deps = [
        ":numerical_diff",
        "//external:gtest",
    ]
)
