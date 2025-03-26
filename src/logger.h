#pragma once

#include <string>
#include <format>

enum class LogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_FATAL
};

class Logger {
public:
    static auto get_instance() -> Logger&;

    auto log(LogLevel level, const std::string& message, bool console_logging = true) const -> void;

private:
    Logger();
    static auto get_current_time(const std::string& format) -> std::string;
    static auto get_log_level_text(LogLevel level) -> std::string;

    std::string log_files_dir;
    std::string log_file_name;
};

#ifdef DEBUG
#define LOG_DEBUG(msg, ...) Logger::get_instance().log(LogLevel::LOG_DEBUG, std::format(msg __VA_OPT__(,) __VA_ARGS__));
#else
#define LOG_DEBUG(msg, ...)
#endif

#define LOG_INFO(msg, ...) Logger::get_instance().log(LogLevel::LOG_INFO, std::format(msg __VA_OPT__(,) __VA_ARGS__));
#define LOG_WARNING(msg, ...) Logger::get_instance().log(LogLevel::LOG_WARNING, std::format(msg __VA_OPT__(,) __VA_ARGS__));
#define LOG_ERROR(msg, ...) Logger::get_instance().log(LogLevel::LOG_ERROR, std::format(msg __VA_OPT__(,) __VA_ARGS__));
#define LOG_FATAL(msg, ...) Logger::get_instance().log(LogLevel::LOG_FATAL, std::format(msg __VA_OPT__(,) __VA_ARGS__));
