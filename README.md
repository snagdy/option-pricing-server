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

Just start it via the following bazel command. Note that it doesn't currently do anything yet.

```bash
bazel run //src:option_pricing_server
```

### Future Enhancements

- Extending the option_pricing_server.cpp to run a gRPC service accepting the protobufs from 
[snagdy/finance_protos](https://github.com/snagdy/finance_protos) and using them to vend option premiums from the default Black-Scholes pricing model.

- Adding additional pricing models, and the ability to select which is used for vending an option premium.

- The ability to submit batch requests for vectors of inputs to calculate grids of option data.

# License

This project is licensed under the MIT License - see the LICENSE file for detials.