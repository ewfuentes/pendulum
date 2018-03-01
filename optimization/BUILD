package(default_visibility = ["//visibility:public"])

cc_library(
    name = "optimizer",
    hdrs = ["optimizer.hh"],
    deps = ["//external:eigen"],
)

cc_library(
    name = "test_functions",
    hdrs = ["test_functions.hh"],
    deps = ["//external:eigen"],
)

cc_library(
    name = "gradient_descent",
    hdrs = ["gradient_descent.hh"],
    srcs = ["gradient_descent.cc"],
    deps = [":optimizer", "//util:numerical_diff",],
)

cc_test(
    name = "gradient_descent_test",
    srcs = ["gradient_descent_test.cc"],
    deps = [
        ":gradient_descent",
        ":test_functions",
        "//external:gtest",
    ],
)
