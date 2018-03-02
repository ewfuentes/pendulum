package(default_visibility = ["//visibility:public"])

cc_library(
    name = "naive_ilqr",
    hdrs = [
        "naive_ilqr.hh",
        "naive_ilqr_impl.hh",
    ],
    deps = [
        "//costs:cost_function",
        "//dynamics:dynamical_system",
    ],
)

cc_test(
    name = "naive_ilqr_test",
    srcs = ["naive_ilqr_test.cc"],
    deps = [
        ":naive_ilqr",
        "//costs:pendulum_costs",
        "//dynamics:pendulum",
        "//optimization:gradient_descent",
        "//external:gtest",
        "//util:numerical_diff",
    ]
)
