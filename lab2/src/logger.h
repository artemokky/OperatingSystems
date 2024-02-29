#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>

class Logger {
public:
    Logger(const std::string& logFile);
    ~Logger();

    void LogInfo(const std::string& message);
    void LogError(const std::string& message);

    static Logger& GetInstance(const std::string& logFile = "log.txt");

private:
    std::ofstream logStream;
    std::mutex logMutex;

    static std::string CurrentTime();

    static Logger* instance;
};

#endif // LOGGER_H
