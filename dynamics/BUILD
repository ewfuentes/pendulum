cc_library(
    name = "dynamical_system",
    hdrs = ["dynamical_system.hh"],
    deps = [
        "//external:eigen",
    ],
)

cc_library(
    name = "pendulum",
    hdrs = ["pendulum.hh"],
    srcs = ["pendulum.cc"],
    deps = [
        ":dynamical_system"
    ],
)

cc_test(
    name = "pendulum_test",
    srcs = ["pendulum_test.cc"],
    deps = [
        ":pendulum",
        "//external:gtest",
    ]
)
