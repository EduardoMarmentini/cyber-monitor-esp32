#include "logger.h"
#include <stdarg.h>

unsigned long Logger::startTime = 0;

void Logger::begin() {
    startTime = millis();
}

void Logger::info(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log("INFO", format, args);
    va_end(args);
}

void Logger::warn(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log("WARN", format, args);
    va_end(args);
}

void Logger::error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log("ERROR", format, args);
    va_end(args);
}

void Logger::debug(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log("DEBUG", format, args);
    va_end(args);
}

void Logger::log(const char* level, const char* format, va_list args) {
    unsigned long elapsed = millis() - startTime;
    unsigned long minutes = elapsed / 60000;
    unsigned long seconds = (elapsed % 60000) / 1000;
    unsigned long millis_val = elapsed % 1000;

    char prefix[32];
    sprintf(prefix, "[%02lu:%02lu.%03lu] [%s] ", minutes, seconds, millis_val, level);
    Serial.print(prefix);

    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    Serial.println(buffer);
}
