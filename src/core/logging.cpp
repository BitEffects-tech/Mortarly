#include "logging.h"

namespace logging {
    std::vector<std::shared_ptr<Logger>> loggers;

    static LogLevel currentLogLevel = LogLevel::Error;
    void setLogLevel(LogLevel level) {
        currentLogLevel = level;
    }

    void write(LogLevel level, const std::string& msg) {
        if (level <= currentLogLevel){
            for (auto& logger : loggers) {
                logger->write(msg);
            }
        }
    }
}

class TestLogger : public logging::Logger {
public:
    std::vector<std::string> messages;

    void write(const std::string &str) override {
        messages.push_back(str);
    }
};

TEST(LOGGING, ErrorLogLevel) {
    auto logger = std::make_shared<TestLogger>();
    logging::loggers.push_back(logger);
    logging::setLogLevel(logging::LogLevel::Error);

    logging::error("Error Message");
    logging::info("Info Message");
    logging::debug("Debug Message");

    ASSERT_EQ(logger->messages.size(), 1);
}

TEST(LOGGING, InfoLogLevel) {
    auto logger = std::make_shared<TestLogger>();
    logging::loggers.push_back(logger);
    logging::setLogLevel(logging::LogLevel::Info);

    logging::error("Error Message");
    logging::info("Info Message");
    logging::debug("Debug Message");

    ASSERT_EQ(logger->messages.size(), 2);
}

TEST(LOGGING, DebugLogLevel) {
    auto logger = std::make_shared<TestLogger>();
    logging::loggers.push_back(logger);
    logging::setLogLevel(logging::LogLevel::Debug);

    logging::error("Error Message");
    logging::info("Info Message");
    logging::debug("Debug Message");

    ASSERT_EQ(logger->messages.size(), 3);
}
