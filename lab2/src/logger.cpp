#include "logger.h"
#include <iostream>
#include <ctime>
#include <sstream>

Logger* Logger::instance = nullptr;

Logger::Logger(const std::string& logFile) {
    logStream.open(logFile, std::ios::out | std::ios::app);
    if (!logStream.is_open()) {
        throw std::runtime_error("Unable to open log file: " + logFile);
    }
}

Logger::~Logger() {
    if (logStream.is_open()) {
        logStream.close();
    }
}

void Logger::LogInfo(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    logStream << CurrentTime() << " [INFO]: " << message << std::endl;
}

void Logger::LogError(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    logStream << CurrentTime() << " [ERROR]: " << message << std::endl;
}

Logger& Logger::GetInstance(const std::string& logFile) {
    if (instance == nullptr) {
        instance = new Logger(logFile);
    }
    return *instance;
}

std::string Logger::CurrentTime() {
    std::time_t now = std::time(nullptr);
    std::tm *ltm = std::localtime(&now);
    std::ostringstream oss;
    oss << 1900 + ltm->tm_year << '-'
        << 1 + ltm->tm_mon << '-'
        << ltm->tm_mday << ' '
        << ltm->tm_hour << ':'
        << ltm->tm_min << ':'
        << ltm->tm_sec;
    return oss.str();
}
