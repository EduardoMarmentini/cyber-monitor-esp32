#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ESPAsyncWebServer.h>
#include "esp_task_wdt.h"

#include "config.h"
#include "logger.h"
#include "wifi_scanner.h"
#include "api_routes.h"
#include "websocket.h"
#include "services/wifi_service.h"
#include "services/stats_service.h"

AsyncWebServer server(API_PORT);

unsigned long lastWifiScanTime = 0;
unsigned long lastReconnectAttempt = 0;

void loadWifiConfig() {
    WiFi.mode(WIFI_STA);
    LOG_INFO("Using WiFi config from config.h: %s", WIFI_SSID);

    if (strlen(WIFI_PASSWORD) == 0) {
        WiFi.begin(WIFI_SSID);
    } else {
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    }
}

void setupWiFi() {
    LOG_INFO("Connecting to WiFi...");
    esp_task_wdt_reset();

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < WIFI_CONNECT_RETRIES) {
        delay(WIFI_RETRY_INTERVAL);
        attempts++;
        esp_task_wdt_reset();
        LOG_INFO("Connecting... (attempt %d/%d)", attempts, WIFI_CONNECT_RETRIES);
    }
    esp_task_wdt_reset();

    if (WiFi.status() == WL_CONNECTED) {
        LOG_INFO("WiFi connected! IP: %s", WiFi.localIP().toString().c_str());
        LOG_INFO("Trying to start mDNS...");

        if (MDNS.begin(MDNS_HOSTNAME)) {
            MDNS.addService("http", "tcp", API_PORT);
            LOG_INFO("mDNS responder started: http://%s.local", MDNS_HOSTNAME);
            LOG_INFO("mDNS setup SUCCESS!");
        } else {
            LOG_ERROR("mDNS setup FAILED - will use IP instead: http://%s:8080", WiFi.localIP().toString().c_str());
        }
    } else {
        LOG_ERROR("WiFi connection failed after %d attempts", WIFI_CONNECT_RETRIES);
    }
}

void setupServer() {
    LOG_INFO("Initializing HTTP Server on port %d", API_PORT);
    esp_task_wdt_reset();

    ApiRoutes::begin(server);

    server.onNotFound([](AsyncWebServerRequest* request) {
        if (request->method() == HTTP_OPTIONS) {
            AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", "");
            response->addHeader("Access-Control-Allow-Origin", "*");
            response->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
            response->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
            request->send(response);
            return;
        }
        LOG_WARN("404 - %s", request->url().c_str());
        AsyncWebServerResponse *response = request->beginResponse(404, "application/json", "{\"error\":\"Not Found\"}");
        response->addHeader("Access-Control-Allow-Origin", "*");
        request->send(response);
    });

    server.begin();
    esp_task_wdt_reset();
    LOG_INFO("HTTP Server started successfully on port %d", API_PORT);
}

void initWatchdog() {
    esp_task_wdt_init(WATCHDOG_TIMEOUT_SEC, true);
    esp_task_wdt_add(NULL);
    LOG_INFO("Watchdog initialized (timeout: %ds)", WATCHDOG_TIMEOUT_SEC);
}

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    delay(2000);

    Logger::begin();
    Logger::setLevel(LOG_LEVEL_INFO);

    LOG_INFO("System starting...");

    initWatchdog();
    esp_task_wdt_reset();

    loadWifiConfig();
    setupWiFi();
    esp_task_wdt_reset();

    WifiScanner::begin();
    esp_task_wdt_reset();
    
    delay(500);
    
    StatsService::begin();
    esp_task_wdt_reset();
    
    setupServer();
    esp_task_wdt_reset();
    
    WebSocketServer::begin(server);
    esp_task_wdt_reset();
    
    WifiService::begin();
    esp_task_wdt_reset();
    
    delay(1000);
    
    WifiScanner::onScanComplete([](const std::vector<WifiNetwork>& networks) {
        WifiService::update();
    });

    WifiService::onNetworkFound([](const WifiNetwork& network) {
        WebSocketServer::broadcastEvent("WIFI_UPDATE", network);
    });

    lastWifiScanTime = millis();
    lastReconnectAttempt = millis();

    LOG_INFO("Setup complete. Entering main loop...");
    esp_task_wdt_reset();
}

void loop() {
    esp_task_wdt_reset();

    unsigned long currentTime = millis();

    if (currentTime - lastWifiScanTime >= WIFI_SCAN_INTERVAL) {
        WifiScanner::scan();
        lastWifiScanTime = currentTime;
        esp_task_wdt_reset();
    }

    if (WiFi.status() != WL_CONNECTED) {
        if (currentTime - lastReconnectAttempt >= 10000) {
            LOG_WARN("WiFi disconnected, attempting reconnect...");
            WiFi.disconnect();
            WiFi.reconnect();
            lastReconnectAttempt = currentTime;
        }
    } else {
        static bool connectedLogged = false;
        if (!connectedLogged) {
            LOG_INFO("WiFi connected! IP: %s", WiFi.localIP().toString().c_str());
            connectedLogged = true;
        }
    }

    delay(100);
}
