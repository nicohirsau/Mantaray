#include <string>

namespace MR {
class Logger {
    public:
        enum LogLevel {
            LOG_INFO,
            LOG_WARNING,
            LOG_ERROR
        };

        Logger(std::string name);
        void Log(std::string message, Logger::LogLevel logLevel = LOG_INFO);
        static void Log(std::string name, std::string message, Logger::LogLevel logLevel = LOG_INFO);
        
    private:
        std::string m_name = "";
};
}