#!/bin/bash
set -e

# SSH setup
if [ -d ~/.ssh ]; then
  chmod 700 ~/.ssh
  chmod 600 ~/.ssh/id_rsa*
  ssh-keyscan -H github.com >> ~/.ssh/known_hosts
  echo "SSH configurations complete; keys copied into devcontainer"
else
  echo "Skipping SSH configuration; no keys found."
fi

# Create target directories for Bazel
mkdir -p /workspaces/option_pricing_server/proto/finance/options

# Copy generated files from the Dockerfile's temporary location
if [ -d "/tmp/proto" ] && [ "$(ls -A /tmp/proto)" ]; then
  echo "Copying .proto from temporary location..."
  cp -r /tmp/proto/* /workspaces/option_pricing_server/proto/finance/options/
  echo ".proto files copied successfully"
else
  echo "Error: .proto files not found in /tmp/proto"
  exit 1
fi

# Create BUILD file for the proto directory 
# necessary as this need to be generated on devcontainer creation
cat > /workspaces/option_pricing_server/proto/finance/options/BUILD << 'EOF'
load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")

# Generated headers and source files only
proto_library(
    name = "black_scholes_proto",
    srcs = ["black_scholes.proto"],
)

cc_proto_library(
    name = "black_scholes_cc_proto",
    deps = [":black_scholes_proto"],
    visibility = ["//visibility:public"],
)

cc_grpc_library(
    name = "black_scholes_cc_grpc",
    srcs = [":black_scholes_proto"],
    grpc_only = True,
    deps = [":black_scholes_cc_proto"],
    visibility = ["//visibility:public"],
)
EOF

echo "Created BUILD file in finance/options directory"