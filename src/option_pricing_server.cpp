#include "black_scholes_pricer.h"
#include "protos/finance/options/black_scholes.pb.h"
#include "protos/finance/options/black_scholes.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>
#include <iostream>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using finance::options::BlackScholesParameters;
using finance::options::OptionPricingRequest;
using finance::options::OptionPricingResponse;
using finance::options::OptionPricingService;
using finance::options::OptionType;

// Implementation of the OptionPricingService
class OptionPricingServiceImpl final : public OptionPricingService::Service {
    private:
        BlackScholesOptionPricer pricer;

public:
    // Implement the RPC method defined in the proto file
    Status CalculateOptionPrice(ServerContext* context, 
                               const OptionPricingRequest* request,
                               OptionPricingResponse* response) override {
        // Extract parameters from the request
        const BlackScholesParameters& params = request->parameters();
        double S = params.stock_price();
        double K = params.strike_price();
        double r = params.risk_free_rate();
        double sigma = params.volatility();
        double T = params.time_to_maturity();
        OptionType option_type = params.option_type();

        // Validate inputs to avoid potential errors
        if (S <= 0 || K <= 0 || sigma <= 0 || T <= 0) {
            return Status(grpc::StatusCode::INVALID_ARGUMENT, 
                         "Invalid parameters: Stock price, strike price, volatility, and time to maturity must be positive");
        }

        // Calculate the option price based on the option type
        double option_price = 0.0;
        switch (option_type) {
            case OptionType::CALL:
                option_price = pricer.calculateCall(S, K, r, sigma, T);
                break;
            case OptionType::PUT:
                option_price = pricer.calculatePut(S, K, r, sigma, T);
                break;
            default:
                return Status(grpc::StatusCode::INVALID_ARGUMENT, "Unknown option type");
        }

        // Set the response
        response->set_option_price(option_price);
        response->set_option_type(option_type);

        // Return success status
        return Status::OK;
    }
};


void RunServer(const std::string& server_address) {
    OptionPricingServiceImpl service;

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with clients
    builder.RegisterService(&service);
    
    // Assemble and start the server
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown
    server->Wait();
}

int main(int argc, char** argv) {
    // TODO(snagdy): change this to read it from a configuration file.
    // Default to localhost:50051 if no address specified
    std::string server_address = "0.0.0.0:50051";
    
    if (argc > 1) {
        server_address = argv[1];
    }

    RunServer(server_address);
    return 0;
}