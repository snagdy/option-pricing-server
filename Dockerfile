# Build stage
FROM debian:12-slim AS builder

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    openjdk-17-jre \
    python3 \
    python3-pip \
    curl \
    wget \
    git \
    unzip \
    zip \
    apt-transport-https \
    gnupg \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Install Bazelisk for Bazel build
ARG BAZELISK_VERSION=v1.10.1
RUN ARCH=$(uname -m) && \
    if [ "$ARCH" = "x86_64" ]; then ARCH="amd64"; \
    elif [ "$ARCH" = "aarch64" ]; then ARCH="arm64"; \
    else echo "Unsupported architecture: $ARCH" && exit 1; fi && \
    wget https://github.com/bazelbuild/bazelisk/releases/download/${BAZELISK_VERSION}/bazelisk-linux-${ARCH} -O /usr/local/bin/bazel && \
    chmod +x /usr/local/bin/bazel

# Compile the source
WORKDIR /src

# Copy only files necessary for dependency resolution first for better caching
COPY MODULE.bazel MODULE.bazel.lock ./

# Copy the rest of the source code
COPY . .

RUN bazel build //src:option_pricing_server


# Runtime stage
FROM debian:12-slim AS runtime

# Install only essential runtime dependencies
RUN apt-get update && apt-get install -y \
    openjdk-17-jre \
    python3 \
    && apt-get clean && rm -rf /var/lib/apt/lists/*


WORKDIR /app

# Copy the binary from the builder to the runtime stage
COPY --from=builder /src/bazel-bin/src/option_pricing_server /app/option_pricing_server

# Copy the configs from the builder to the runtime stage in a location expected by the binary
RUN mkdir -p /app/option_pricing_server.runfiles/option_pricing_server/conf
# We copy to the runtime runfile path
COPY --from=builder /src/conf/server_config.json /app/option_pricing_server.runfiles/option_pricing_server/conf/server_config.json

RUN chmod +x option_pricing_server

EXPOSE 8080

ENTRYPOINT [ "./option_pricing_server" ]