load("@rules_cc//cc:defs.bzl", "cc_library", "cc_binary")

cc_library(
    name = "black_scholes_pricer_lib",
    srcs = ["black_scholes_pricer.cpp"],
    hdrs = ["black_scholes_pricer.h"],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "option_pricing_server",
    srcs = ["option_pricing_server.cpp"],
    deps = [
        ":black_scholes_pricer_lib",
        "//protos/finance/options:black_scholes_cc_proto",
        "//protos/finance/options:black_scholes_cc_grpc",
        "@grpc//:grpc++",
        "@grpc//:grpc",
    ],
    includes = ["."]
)
