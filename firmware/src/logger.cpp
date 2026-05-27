#include "logger.h"
#include <stdarg.h>

unsigned long Logger::startTime = 0;
LogLevel Logger::currentLevel = LOG_LEVEL_DEBUG;

void Logger::begin() {
    startTime = millis();
}

void Logger::setLevel(LogLevel level) {
    currentLevel = level;
}

bool Logger::shouldLog(LogLevel level) {
    return level >= currentLevel;
}

void Logger::debug(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(LOG_LEVEL_DEBUG, "DEBUG", format, args);
    va_end(args);
}

void Logger::info(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(LOG_LEVEL_INFO, "INFO", format, args);
    va_end(args);
}

void Logger::warn(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(LOG_LEVEL_WARN, "WARN", format, args);
    va_end(args);
}

void Logger::error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(LOG_LEVEL_ERROR, "ERROR", format, args);
    va_end(args);
}

void Logger::critical(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(LOG_LEVEL_CRITICAL, "CRITICAL", format, args);
    va_end(args);
}

void Logger::log(LogLevel level, const char* label, const char* format, va_list args) {
    if (!shouldLog(level)) return;

    unsigned long elapsed = millis() - startTime;
    unsigned long minutes = elapsed / 60000;
    unsigned long seconds = (elapsed % 60000) / 1000;

    char prefix[32];
    sprintf(prefix, "[%02lu:%02lu] [%s] ", minutes, seconds, label);
    Serial.print(prefix);

    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    Serial.println(buffer);
}
