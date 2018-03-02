package(default_visibility = ["//visibility:public"])

cc_library(
    name = "cost_function",
    hdrs = ["cost_function.hh"],
    deps = ["//dynamics:dynamical_system",],
)

cc_library(
    name = "pendulum_costs",
    hdrs = ["pendulum_costs.hh"],
    deps = [
        "//dynamics:dynamical_system",
        "//util:angle_diff",
        "//util:huber",
    ],
)
