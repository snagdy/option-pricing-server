# Option Pricing Server

A C++ option pricing server using the Black-Scholes model.

## Setup

This project uses Bazel for building and managing dependencies. It has an included Dockerfile for a devcontainer workflow.

### Prerequisites

- Bazel
- C++ compiler
- Protocol Buffers

### Building in Devcontainer

Currently, you do this from within the devcontainer. A task has been defined to do this also.

```bash
bazel build //src:option_pricing_server
```

### Running in Devcontainer

Just start it via the following bazel command. Note that it doesn't currently do anything yet.

```bash
bazel run //src:option_pricing_server
```
## Building via Docker

```bash
docker build -t option-pricing-server .
```

### Running via Docker

This assumes you've configured the container port exposed, and the application config port to 8080

```bash
docker run --name option-pricing-server option-pricing-server:latest -d -p 8080:8080
```

### Future Enhancements

- Adding an implied vol gRPC service

- Adding additional pricing models, and the ability to select which is used for vending an option premium.

- The ability to submit batch requests for vectors of inputs to calculate grids of option data.

# License

This project is licensed under the MIT License - see the LICENSE file for detials.