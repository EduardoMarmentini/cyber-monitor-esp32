#pragma once

#include <Arduino.h>

class Logger {
public:
    static void begin();
    static void info(const char* format, ...);
    static void warn(const char* format, ...);
    static void error(const char* format, ...);

private:
    static void log(const char* level, const char* format, va_list args);
    static unsigned long startTime;
};

// Convenience macros
#define LOG_INFO(fmt, ...) Logger::info(fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) Logger::warn(fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) Logger::error(fmt, ##__VA_ARGS__)
