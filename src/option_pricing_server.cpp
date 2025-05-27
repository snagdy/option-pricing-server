#include <grpcpp/grpcpp.h>
#include <spdlog/spdlog.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

#include "black_scholes_options.h"
#include "proto/finance/options/black_scholes.grpc.pb.h"
#include "proto/finance/options/black_scholes.pb.h"
#include "tools/cpp/runfiles/runfiles.h"

using finance::options::BlackScholesParameters;
using finance::options::BlackScholesImpliedVolParameters;
using finance::options::OptionPricingRequest;
using finance::options::OptionImpliedVolRequest;
using finance::options::OptionPricingResponse;
using finance::options::OptionImpliedVolResponse;
using finance::options::OptionPricingService;
using finance::options::OptionType;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

// Implementation of the OptionPricingService
class OptionPricingServiceImpl final : public OptionPricingService::Service {
   public:
    // Implement the RPC method defined in the proto file
    Status CalculateOptionPrice(ServerContext* context, const OptionPricingRequest* request,
                                OptionPricingResponse* response) override {
        // Extract parameters from the request
        const BlackScholesParameters& params = request->parameters();
        double S = params.stock_price();
        double K = params.strike_price();
        double r = params.risk_free_rate();
        double q = params.dividend_rate();
        double sigma = params.volatility();
        double T = params.time_to_maturity();
        OptionType option_type = params.option_type();

        // Validate inputs to avoid potential errors
        if (S <= 0 || K <= 0 || q < 0 || sigma <= 0 || T <= 0) {
            std::string invalid_argument_message =
                "Invalid parameters: Stock price, strike price, volatility, and time to maturity "
                "must be positive, and dividend must be non-negative";
            spdlog::error(invalid_argument_message);
            return Status(grpc::StatusCode::INVALID_ARGUMENT, invalid_argument_message);
        }

        // Calculate the option price based on the option type
        double option_price = 0.0, option_delta = 0.0, option_gamma = 0.0, option_vega = 0.0;
        switch (option_type) {
            case OptionType::CALL: {
                BlackScholesCall blackScholesCall{S, K, r, q, sigma, T};
                option_price = blackScholesCall.price;
                option_delta = blackScholesCall.delta;
                option_gamma = blackScholesCall.gamma;
                option_vega = blackScholesCall.vega;
                break;
            }
            case OptionType::PUT: {
                BlackScholesPut blackScholesPut{S, K, r, q, sigma, T};
                option_price = blackScholesPut.price;
                option_delta = blackScholesPut.delta;
                option_gamma = blackScholesPut.gamma;
                option_vega = blackScholesPut.vega;
                break;
            }
            default:
                std::string unknown_option_message = "Unknown option type";
                spdlog::error(unknown_option_message);
                return Status(grpc::StatusCode::INVALID_ARGUMENT, unknown_option_message);
        }

        // Set the response
        response->set_option_price(option_price);
        response->set_option_type(option_type);
        response->set_delta(option_delta);
        response->set_gamma(option_gamma);
        response->set_vega(option_vega);

        // Return success status
        return Status::OK;
    }

    Status CalculateImpliedVol(ServerContext* context, const OptionImpliedVolRequest* request,
                               OptionImpliedVolResponse* response) override {
        const BlackScholesImpliedVolParameters& params = request->parameters();
        double P = params.option_premium();
        double S = params.stock_price();
        double K = params.strike_price();
        double r = params.risk_free_rate();
        double q = params.dividend_rate();
        double T = params.time_to_maturity();
        OptionType option_type = params.option_type();

        if (S <= 0 || K <= 0 || q < 0 || P <= 0 || T <= 0) {
            std::string invalid_argument_message =
                "Invalid parameters: Stock price, strike price, option premium, and time to maturity "
                "must be positive, and dividend must be non-negative";
            spdlog::error(invalid_argument_message);
            return Status(grpc::StatusCode::INVALID_ARGUMENT, invalid_argument_message);
        }
        double option_implied_vol = 0.0, option_delta = 0.0, option_gamma = 0.0, option_vega = 0.0;
        switch (option_type) {
            case OptionType::CALL: {
                BlackScholesCallImpliedVol blackScholesCallImpliedVol{P, S, K, r, q, T};
                option_implied_vol = blackScholesCallImpliedVol.implied_vol;
                option_delta = blackScholesCallImpliedVol.delta;
                option_gamma = blackScholesCallImpliedVol.gamma;
                option_vega = blackScholesCallImpliedVol.vega;
                break;
            }
            case OptionType::PUT: {
                BlackScholesPutImpliedVol blackScholesPutImpliedVol{P, S, K, r, q, T};
                option_implied_vol = blackScholesPutImpliedVol.implied_vol;
                option_delta = blackScholesPutImpliedVol.delta;
                option_gamma = blackScholesPutImpliedVol.gamma;
                option_vega = blackScholesPutImpliedVol.vega;
                break;
            }
            default:
                std::string unknown_option_message = "Unknown option type";
                spdlog::error(unknown_option_message);
                return Status(grpc::StatusCode::INVALID_ARGUMENT, unknown_option_message);
        }

        response->set_implied_volatility(option_implied_vol);
        response->set_option_type(option_type);
        response->set_delta(option_delta);
        response->set_gamma(option_gamma);
        response->set_vega(option_vega);

        return Status::OK;
    }
};



void runServer(const std::string& serverAddressAndPort) {
    OptionPricingServiceImpl service;

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism
    builder.AddListeningPort(serverAddressAndPort, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with clients
    builder.RegisterService(&service);

    // Assemble and start the server
    std::unique_ptr<Server> server(builder.BuildAndStart());
    spdlog::info("Server listening on {}", serverAddressAndPort);

    // Wait for the server to shutdown
    server->Wait();
}

struct Config {
    std::string serverAddress;
    int port;
};

Config initialiseConfigs(const std::string& argv0) {
    using bazel::tools::cpp::runfiles::Runfiles;

    std::string error;
    std::unique_ptr<Runfiles> runfiles(Runfiles::Create(argv0, &error));
    if (!runfiles) {
        // std::cerr << "Failed to create Runfiles: " << error << "\n";
        spdlog::critical("Failed to create Runfiles: {}", error);
        std::exit(EXIT_FAILURE);
    }

    const std::string CONFIG_FILEPATH = runfiles->Rlocation("option_pricing_server/conf/server_config.json");
    spdlog::info("Config file expected at: {}", CONFIG_FILEPATH);
    std::ifstream file(CONFIG_FILEPATH);
    if (!file.is_open()) {
        spdlog::critical("Unable to open config file.");
        std::exit(EXIT_FAILURE);
    }

    nlohmann::json configJson;
    try {
        file >> configJson;

        if (!configJson.contains("server") || !configJson["server"].contains("address") ||
            !configJson["server"].contains("port")) {
            spdlog::critical(
                "Required configuration fields missing, check for server, server.address, and "
                "server.port in {}",
                CONFIG_FILEPATH);
        }

        Config configStruct;
        configStruct.serverAddress = configJson["server"]["address"];
        configStruct.port = configJson["server"]["port"];
        return configStruct;
    } catch (nlohmann::json::parse_error& e) {
        spdlog::critical("Parsing JSON config failed. JSON parse error: {}", e.what());
        std::exit(EXIT_FAILURE);
    }
}

int main(int argc, char** argv) {
    Config configStruct = initialiseConfigs(argv[0]);
    std::string serverAddressAndPort =
        configStruct.serverAddress + ":" + std::to_string(configStruct.port);

    runServer(serverAddressAndPort);
    return 0;
}