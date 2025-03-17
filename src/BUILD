load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "black_scholes_pricer",
    srcs = ["black_scholes_pricer.cpp"],
    hdrs = ["black_scholes_pricer.h"],
    visibility = ["//visibility:public"],
    strip_include_prefix = ".",
)

cc_binary(
    name = "option_pricing_server",
    srcs = ["option_pricing_server.cpp"],
    deps = [
        ":black_scholes_pricer",
        "//protos:option_pricing_proto_cc",
        "//gen:finance_proto_headers",
    ],
    includes = ["."]
)
