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

# Create directories for proto files
mkdir -p /workspaces/option_pricing_server/protos
mkdir -p /workspaces/option_pricing_server/gen/finance/options

# Check if proto files exist in tmp directory
if [ -d "/tmp/proto_files" ] && [ "$(ls -A /tmp/proto_files)" ]; then
  echo "Copying proto files from temporary location..."
  cp -r /tmp/proto_files/* /workspaces/option_pricing_server/protos/
else
  echo "Warning: Proto files not found in /tmp/proto_files"
  
  # Alternative: Try to copy directly from the image
  echo "Attempting to copy proto files directly..."
  docker run --rm --entrypoint /bin/sh ghcr.io/snagdy/finance_protos:sha-03df67e -c "cat /protos/options/black_scholes.proto" > /workspaces/option_pricing_server/protos/black_scholes.proto
  if [ $? -eq 0 ]; then
    echo "Successfully copied proto file directly from image"
  else
    echo "Failed to copy proto file. Please check image and paths."
  fi
fi

# Check if generated files exist in tmp directory
if [ -d "/tmp/gen_files" ] && [ "$(ls -A /tmp/gen_files)" ]; then
  echo "Copying generated files to gen/finance/options structure..."
  mkdir -p /workspaces/option_pricing_server/gen/finance/options
  cp -r /tmp/gen_files/* /workspaces/option_pricing_server/gen/finance/options/
else
  echo "Warning: Generated files not found in /tmp/gen_files"
fi

# Create BUILD file for protos directory
cat > /workspaces/option_pricing_server/protos/BUILD << 'EOF'
load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")

proto_library(
    name = "option_pricing_proto",
    srcs = glob(["*.proto"]),
    visibility = ["//visibility:public"],
)

cc_proto_library(
    name = "option_pricing_proto_cc",
    visibility = ["//visibility:public"],
    deps = [":option_pricing_proto"],
)
EOF

# Create BUILD file for gen directory
mkdir -p /workspaces/option_pricing_server/gen
cat > /workspaces/option_pricing_server/gen/BUILD << 'EOF'
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "finance_proto_headers",
    hdrs = glob(["**/*.pb.h", "**/*.pb.cc"]),
    includes = ["."],
    strip_include_prefix = "/gen",  # This strips "gen" from include paths
)
EOF

echo "Created BUILD files in protos and gen directories"