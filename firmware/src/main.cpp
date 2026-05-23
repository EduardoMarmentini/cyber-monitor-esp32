#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#include "config.h"
#include "logger.h"
#include "wifi_scanner.h"
#include "api_routes.h"

// ============================================================================
// Global Objects
// ============================================================================
AsyncWebServer server(API_PORT);
unsigned long lastWifiScanTime = 0;

// ============================================================================
// WiFi Setup
// ============================================================================
void setupWiFi() {
    LOG_INFO("Connecting to WiFi: %s", WIFI_SSID);
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < WIFI_CONNECT_RETRIES) {
        delay(WIFI_RETRY_INTERVAL);
        attempts++;
        LOG_INFO("Connecting... (attempt %d/%d)", attempts, WIFI_CONNECT_RETRIES);
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        LOG_INFO("WiFi connected! IP: %s", WiFi.localIP().toString().c_str());
    } else {
        LOG_ERROR("WiFi connection failed!");
    }
}

// ============================================================================
// Server Setup
// ============================================================================
void setupServer() {
    LOG_INFO("Initializing HTTP Server on port %d", API_PORT);
    
    ApiRoutes::begin(server);
    
    server.onNotFound([](AsyncWebServerRequest* request) {
        LOG_WARN("404 - %s", request->url().c_str());
        request->send(404, "application/json", "{\"error\":\"Not Found\"}");
    });
    
    server.begin();
    LOG_INFO("HTTP Server started");
}

// ============================================================================
// Setup
// ============================================================================
void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    delay(1000);  // Give serial time to initialize
    
    LOG_INFO("System starting...");
    
    setupWiFi();
    WifiScanner::begin();
    setupServer();
    
    lastWifiScanTime = millis();
    
    LOG_INFO("Setup complete. Entering main loop...");
}

// ============================================================================
// Main Loop
// ============================================================================
void loop() {
    // Perform WiFi scan at interval
    unsigned long currentTime = millis();
    if (currentTime - lastWifiScanTime >= WIFI_SCAN_INTERVAL) {
        WifiScanner::scan();
        lastWifiScanTime = currentTime;
    }
    
    // Keep WiFi alive
    if (WiFi.status() != WL_CONNECTED) {
        LOG_WARN("WiFi disconnected, attempting reconnect...");
        WiFi.reconnect();
    }
    
    delay(100);  // Small delay to prevent watchdog issues
}
