# Option Pricing Server

A C++ option pricing server using the Black-Scholes model.

## Setup

This project uses Bazel for building and managing dependencies. It has an included Dockerfile for a devcontainer workflow.

### Prerequisites

- Bazel
- C++ compiler
- Protocol Buffers

### Building

Currently, you do this from within the devcontainer. A task has been defined to do this also.

```bash
bazel build //src:option_pricing_server
```

### Running

Just start it via the following bazel command.

```bash
bazel run //src:option_pricing_server
```