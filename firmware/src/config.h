#pragma once

// ============================================================================
// WiFi Configuration
// ============================================================================
#define WIFI_SSID "SUA_REDE_AQUI"
#define WIFI_PASSWORD "SUA_SENHA_AQUI"
#define WIFI_CONNECT_TIMEOUT 10000
#define WIFI_CONNECT_RETRIES 10
#define WIFI_RETRY_INTERVAL 500

// ============================================================================
// API Configuration
// ============================================================================
#define API_PORT 80
#define API_ENDPOINT "/api"

// ============================================================================
// Scanner Timings
// ============================================================================
#define WIFI_SCAN_INTERVAL 10000  // 10 segundos

// ============================================================================
// Serial Configuration
// ============================================================================
#define SERIAL_BAUD_RATE 115200

// ============================================================================
// Logger Configuration
// ============================================================================
#define LOG_LEVEL_DEBUG
#define LOG_LEVEL_INFO
#define LOG_LEVEL_WARN
#define LOG_LEVEL_ERROR
