#pragma once

#include <Arduino.h>

enum LogLevel {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_CRITICAL,
    LOG_LEVEL_NONE
};

class Logger {
public:
    static void begin();
    static void setLevel(LogLevel level);
    static void debug(const char* format, ...);
    static void info(const char* format, ...);
    static void warn(const char* format, ...);
    static void error(const char* format, ...);
    static void critical(const char* format, ...);

private:
    static void log(LogLevel level, const char* label, const char* format, va_list args);
    static bool shouldLog(LogLevel level);
    static unsigned long startTime;
    static LogLevel currentLevel;
};

#define LOG_DEBUG(fmt, ...) Logger::debug(fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) Logger::info(fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) Logger::warn(fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) Logger::error(fmt, ##__VA_ARGS__)
#define LOG_CRITICAL(fmt, ...) Logger::critical(fmt, ##__VA_ARGS__)
