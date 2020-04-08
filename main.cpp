#include <iostream>
#include "aixlog.hpp"
#include "FixApp.h"

int main(int argc, char** argv) {
    AixLog::Log::init<AixLog::SinkCallback>(AixLog::Severity::trace, AixLog::Type::all, [](const AixLog::Metadata& metadata, const std::string& message)
    {
        std::cout << "[" << AixLog::Log::to_string(metadata.severity) << "] " << metadata.timestamp.to_string() << " - " << metadata.function.file << ":" << metadata.function.line << "   " << message << "\n";
    });
    LOG(DEBUG) << "Start fixparser";

    FixApp app(argc, argv);
    return app.run();
}
