#include "logger.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <filesystem>

auto Logger::get_instance() -> Logger& {
    static Logger instance;
    return instance;
}

auto Logger::log(LogLevel level, const std::string& message, bool console_logging) const -> void {
    const std::string logMessage = std::format("[{}] [{}]: {}\n", get_current_time("%Y-%m-%d %H:%M:%S"), get_log_level_text(level), message);

    // File
    std::ofstream ofs(log_files_dir + "/" + log_file_name, std::fstream::app);
    ofs << logMessage;
    ofs.close();

    // Console
    if (console_logging) {
        std::cout << logMessage;
    }
}

Logger::Logger() {
    log_files_dir = "./logs";
    std::filesystem::create_directories(log_files_dir);
    log_file_name = "logfile_" + get_current_time("%Y-%m-%d") + ".log";
    std::filesystem::remove(log_files_dir + "/" + log_file_name);
}

auto Logger::get_current_time(const std::string& format) -> std::string {
    const auto now = std::chrono::system_clock::now();
    const std::time_t time = std::chrono::system_clock::to_time_t(now);
    const std::tm* localTime = std::localtime(&time);

    std::ostringstream oss;
    oss << std::put_time(localTime, format.c_str());
    return oss.str();
}

auto Logger::get_log_level_text(LogLevel level) -> std::string {
    switch (level) {
        case LogLevel::LOG_DEBUG: return "DEBUG";
        case LogLevel::LOG_INFO: return "INFO";
        case LogLevel::LOG_WARNING: return "WARNING";
        case LogLevel::LOG_ERROR: return "ERROR";
        case LogLevel::LOG_FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
}
