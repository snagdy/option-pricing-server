FROM mcr.microsoft.com/vscode/devcontainers/base:debian-11

# Install essential dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    gdb \
    curl \
    tree \
    git \
    python3 \
    python3-pip \
    unzip \
    openjdk-11-jdk \
    wget \
    apt-transport-https \
    gnupg \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Install clang-format only (without the full Clang compiler)
RUN apt-get update && apt-get install -y \
    clang-format \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Create a basic .clang-format configuration file
RUN echo "---\nLanguage: Cpp\nBasedOnStyle: Google\nIndentWidth: 4\nTabWidth: 4\nUseTab: Never\nColumnLimit: 100\nAllowShortFunctionsOnASingleLine: Empty\nAllowShortIfStatementsOnASingleLine: false\nAllowShortLoopsOnASingleLine: false\nBreakBeforeBraces: Attach\nNamespaceIndentation: None\nPointerAlignment: Left\n..." > /.clang-format

# Add a convenient script to format code
RUN echo '#!/bin/bash\nfind "${1:-.}" -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.cc" | xargs clang-format -i' > /usr/local/bin/format-cpp && \
    chmod +x /usr/local/bin/format-cpp

# Install GitHub CLI
RUN curl -fsSL https://cli.github.com/packages/githubcli-archive-keyring.gpg | dd of=/usr/share/keyrings/githubcli-archive-keyring.gpg \
    && chmod go+r /usr/share/keyrings/githubcli-archive-keyring.gpg \
    && echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/githubcli-archive-keyring.gpg] https://cli.github.com/packages stable main" | tee /etc/apt/sources.list.d/github-cli.list > /dev/null \
    && apt-get update \
    && apt-get install -y gh \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Create temporary directories for proto files
RUN mkdir -p /tmp/proto_files /tmp/gen_files

# Copy proto files from separate image into a temporary location
COPY --from=ghcr.io/snagdy/finance_protos:latest /proto/finance/options/ /tmp/proto/

# Verify files were copied correctly
RUN ls -la /tmp/proto_files/ && ls -la /tmp/gen_files/

# Install Bazelisk
ARG BAZELISK_VERSION=v1.10.1
RUN ARCH=$(uname -m) && \
    if [ "$ARCH" = "x86_64" ]; then ARCH="amd64"; \
    elif [ "$ARCH" = "aarch64" ]; then ARCH="arm64"; \
    else echo "Unsupported architecture: $ARCH" && exit 1; fi && \
    wget https://github.com/bazelbuild/bazelisk/releases/download/${BAZELISK_VERSION}/bazelisk-linux-${ARCH} -O /usr/local/bin/bazel && \
    chmod +x /usr/local/bin/bazel