#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ESPAsyncWebServer.h>
#include <Preferences.h>
#include "esp_task_wdt.h"

#include "config.h"
#include "logger.h"
#include "wifi_scanner.h"
#include "api_routes.h"
#include "websocket.h"
#include "services/wifi_service.h"
#include "services/stats_service.h"
#include "services/ble_service.h"

AsyncWebServer server(API_PORT);

unsigned long lastWifiScanTime = 0;
unsigned long lastBleScanTime = 0;
unsigned long lastReconnectAttempt = 0;

void loadWifiConfig() {
    Preferences prefs;
    if (!prefs.begin(NVS_NAMESPACE, true)) {
        LOG_WARN("Failed to open NVS, using default config");
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        return;
    }

    String ssid = prefs.getString(NVS_KEY_SSID, "");
    String password = prefs.getString(NVS_KEY_PASSWORD, "");
    prefs.end();

    WiFi.mode(WIFI_STA);

    if (ssid.length() > 0) {
        LOG_INFO("WiFi config loaded from NVS: %s", ssid.c_str());
        WiFi.begin(ssid.c_str(), password.c_str());
    } else {
        LOG_INFO("Using default WiFi config from config.h");
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    }
}

void setupWiFi() {
    LOG_INFO("Connecting to WiFi...");

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < WIFI_CONNECT_RETRIES) {
        delay(WIFI_RETRY_INTERVAL);
        attempts++;
        esp_task_wdt_reset();
        LOG_INFO("Connecting... (attempt %d/%d)", attempts, WIFI_CONNECT_RETRIES);
    }

    if (WiFi.status() == WL_CONNECTED) {
        LOG_INFO("WiFi connected! IP: %s", WiFi.localIP().toString().c_str());

        if (MDNS.begin(MDNS_HOSTNAME)) {
            MDNS.addService("http", "tcp", API_PORT);
            LOG_INFO("mDNS responder started: http://%s.local", MDNS_HOSTNAME);
        } else {
            LOG_WARN("mDNS setup failed");
        }
    } else {
        LOG_ERROR("WiFi connection failed after %d attempts", WIFI_CONNECT_RETRIES);
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

void initWatchdog() {
    esp_task_wdt_init(WATCHDOG_TIMEOUT_SEC, true);
    esp_task_wdt_add(NULL);
    LOG_INFO("Watchdog initialized (timeout: %ds)", WATCHDOG_TIMEOUT_SEC);
}

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    delay(1000);

    Logger::begin();
    Logger::setLevel(LOG_LEVEL_INFO);

    LOG_INFO("System starting...");

    initWatchdog();

    loadWifiConfig();
    setupWiFi();

    WifiScanner::begin();
    StatsService::begin();
    setupServer();
    WebSocketServer::begin(server);
    WifiService::begin();
    BleService::getInstance().init();

    BleService::getInstance().onNewDevice([](const BleDevice& device) {
        WebSocketServer::broadcastBleEvent("BLE_UPDATE", device);
    });

    WifiScanner::onScanComplete([](const std::vector<WifiNetwork>& networks) {
        WifiService::update();
    });

    WifiService::onNetworkFound([](const WifiNetwork& network) {
        WebSocketServer::broadcastEvent("WIFI_UPDATE", network);
    });

    lastWifiScanTime = millis();
    lastBleScanTime = millis();

    LOG_INFO("Setup complete. Entering main loop...");
}

void loop() {
    esp_task_wdt_reset();

    unsigned long currentTime = millis();

    if (currentTime - lastWifiScanTime >= WIFI_SCAN_INTERVAL) {
        WifiScanner::scan();
        lastWifiScanTime = currentTime;
    }

    if (currentTime - lastBleScanTime >= BLE_SCAN_INTERVAL) {
        BleService::getInstance().scan();
        lastBleScanTime = currentTime;
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
