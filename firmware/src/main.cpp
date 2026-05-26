#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Preferences.h>

#include "config.h"
#include "logger.h"
#include "wifi_scanner.h"
#include "api_routes.h"
#include "websocket.h"
#include "services/wifi_service.h"
#include "services/stats_service.h"

AsyncWebServer server(API_PORT);
unsigned long lastWifiScanTime = 0;
unsigned long lastWifiServiceUpdate = 0;

void loadWifiConfig() {
    Preferences prefs;
    prefs.begin(NVS_NAMESPACE, true);

    String ssid = prefs.getString(NVS_KEY_SSID, "");
    String password = prefs.getString(NVS_KEY_PASSWORD, "");

    prefs.end();

    if (ssid.length() > 0) {
        LOG_INFO("WiFi config loaded from NVS: %s", ssid.c_str());
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid.c_str(), password.c_str());
    } else {
        LOG_INFO("Using default WiFi config from config.h");
        WiFi.mode(WIFI_STA);
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    }
}

void setupWiFi() {
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

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    delay(1000);

    Logger::begin();
    LOG_INFO("System starting...");

    loadWifiConfig();
    setupWiFi();

    WifiScanner::begin();
    StatsService::begin();
    setupServer();
    WebSocketServer::begin(server);

    WifiService::begin();

    WifiScanner::onScanComplete([](const std::vector<WifiNetwork>& networks) {
        WifiService::update();
    });

    WifiService::onNetworkFound([](const WifiNetwork& network) {
        WebSocketServer::broadcastEvent("WIFI_UPDATE", network);
    });

    lastWifiScanTime = millis();
    lastWifiServiceUpdate = millis();

    LOG_INFO("Setup complete. Entering main loop...");
}

void loop() {
    unsigned long currentTime = millis();

    if (currentTime - lastWifiScanTime >= WIFI_SCAN_INTERVAL) {
        WifiScanner::scan();
        lastWifiScanTime = currentTime;
    }

    if (WiFi.status() != WL_CONNECTED) {
        LOG_WARN("WiFi disconnected, attempting reconnect...");
        WiFi.reconnect();
    }

    delay(100);
}
