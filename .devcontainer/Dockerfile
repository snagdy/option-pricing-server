FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install essential dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    curl \
    git \
    python3 \
    python3-pip \
    unzip \
    openjdk-11-jdk \
    wget \
    apt-transport-https \
    gnupg \
    protobuf-compiler \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Install GitHub CLI
RUN curl -fsSL https://cli.github.com/packages/githubcli-archive-keyring.gpg | dd of=/usr/share/keyrings/githubcli-archive-keyring.gpg \
    && chmod go+r /usr/share/keyrings/githubcli-archive-keyring.gpg \
    && echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/githubcli-archive-keyring.gpg] https://cli.github.com/packages stable main" | tee /etc/apt/sources.list.d/github-cli.list > /dev/null \
    && apt-get update \
    && apt-get install -y gh \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Create vscode user
RUN useradd -ms /bin/bash vscode

# Create temporary directories for proto files
RUN mkdir -p /tmp/proto_files /tmp/gen_files

# Copy proto files from separate image into a temporary location
COPY --from=ghcr.io/snagdy/finance_protos:sha-03df67e /protos/options/black_scholes.proto /tmp/proto_files/
COPY --from=ghcr.io/snagdy/finance_protos:sha-03df67e /gen/cpp/options/ /tmp/gen_files/

# Verify files were copied correctly
RUN ls -la /tmp/proto_files/ && ls -la /tmp/gen_files/

# Install GitHub CLI and Bazelisk (shortened for clarity)
ARG BAZELISK_VERSION=v1.10.1
RUN curl -fsSL https://cli.github.com/packages/githubcli-archive-keyring.gpg | dd of=/usr/share/keyrings/githubcli-archive-keyring.gpg && \
    # ... rest of GitHub CLI installation
    ARCH=$(uname -m) && \
    if [ "$ARCH" = "x86_64" ]; then ARCH="amd64"; \
    elif [ "$ARCH" = "aarch64" ]; then ARCH="arm64"; \
    else echo "Unsupported architecture: $ARCH" && exit 1; fi && \
    wget https://github.com/bazelbuild/bazelisk/releases/download/${BAZELISK_VERSION}/bazelisk-linux-${ARCH} -O /usr/local/bin/bazel && \
    chmod +x /usr/local/bin/bazel