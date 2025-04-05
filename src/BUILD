load("@rules_cc//cc:defs.bzl", "cc_library", "cc_binary")

cc_library(
    name = "black_scholes_pricer_lib",
    srcs = ["black_scholes_pricer.cpp"],
    hdrs = ["black_scholes_pricer.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "black_scholes_options_lib",
    srcs = ["black_scholes_options.cpp"],
    hdrs = ["black_scholes_options.h"],
    deps = [":black_scholes_pricer_lib"],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "option_pricing_server",
    srcs = ["option_pricing_server.cpp"],
    deps = [
        ":black_scholes_options_lib",
        "//proto/finance/options:black_scholes_cc_proto",
        "//proto/finance/options:black_scholes_cc_grpc",
        "@grpc//:grpc++",
        "@grpc//:grpc",
        "@spdlog//:spdlog",
        "@nlohmann_json//:json",
    ],
    includes = ["."]
)
