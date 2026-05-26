#include <Preferences.h>
#include <WiFi.h>
#include "config.h"
#include "api_routes.h"
#include "logger.h"
#include "wifi_scanner.h"
#include "services/stats_service.h"
#include "utils/json_helper.h"

void ApiRoutes::begin(AsyncWebServer& server) {
    LOG_INFO("Setting up API routes...");

    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        handleRoot(request);
    });

    server.on("/api/wifi", HTTP_GET, [](AsyncWebServerRequest* request) {
        handleWifiApi(request);
    });

    server.on("/api/stats", HTTP_GET, [](AsyncWebServerRequest* request) {
        handleStatsApi(request);
    });

    server.on("/api/config/wifi", HTTP_GET, [](AsyncWebServerRequest* request) {
        handleConfigWifiGet(request);
    });

    server.on("/api/config/wifi", HTTP_POST, [](AsyncWebServerRequest* request) {
        handleConfigWifiPost(request);
    });

    LOG_INFO("API routes initialized");
}

void ApiRoutes::handleRoot(AsyncWebServerRequest* request) {
    LOG_DEBUG("GET / (health check)");
    request->send(200, "text/plain", "OK");
}

void ApiRoutes::handleWifiApi(AsyncWebServerRequest* request) {
    LOG_DEBUG("GET /api/wifi");

    const auto& networks = WifiScanner::getNetworks();
    String response = JsonHelper::createWifiResponse(networks);

    request->send(200, "application/json", response);
}

void ApiRoutes::handleStatsApi(AsyncWebServerRequest* request) {
    LOG_DEBUG("GET /api/stats");

    Stats stats = StatsService::getStats();
    String response = JsonHelper::createStatsResponse(
        stats.wifiNetworks, stats.bleDevices, stats.uptime);

    request->send(200, "application/json", response);
}

void ApiRoutes::handleConfigWifiGet(AsyncWebServerRequest* request) {
    LOG_DEBUG("GET /api/config/wifi");

    Preferences prefs;
    prefs.begin(NVS_NAMESPACE, true);
    String ssid = prefs.getString(NVS_KEY_SSID, WIFI_SSID);
    prefs.end();

    DynamicJsonDocument doc(128);
    doc["ssid"] = ssid;
    String response;
    serializeJson(doc, response);

    request->send(200, "application/json", response);
}

void ApiRoutes::handleConfigWifiPost(AsyncWebServerRequest* request) {
    LOG_INFO("POST /api/config/wifi");

    if (!request->hasParam("body", true)) {
        request->send(400, "application/json", "{\"success\":false,\"message\":\"Missing body\"}");
        return;
    }

    String body = request->getParam("body", true)->value();
    DynamicJsonDocument doc(256);
    DeserializationError error = deserializeJson(doc, body);

    if (error) {
        LOG_ERROR("Failed to parse JSON: %s", error.c_str());
        request->send(400, "application/json", "{\"success\":false,\"message\":\"Invalid JSON\"}");
        return;
    }

    const char* ssid = doc["ssid"];
    const char* password = doc["password"] | "";

    if (!ssid || strlen(ssid) == 0) {
        request->send(400, "application/json", "{\"success\":false,\"message\":\"SSID is required\"}");
        return;
    }

    Preferences prefs;
    prefs.begin(NVS_NAMESPACE, false);
    prefs.putString(NVS_KEY_SSID, ssid);
    prefs.putString(NVS_KEY_PASSWORD, password);
    prefs.end();

    LOG_INFO("WiFi config saved to NVS. SSID: %s", ssid);

    WiFi.disconnect();
    WiFi.begin(ssid, password);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < WIFI_CONNECT_RETRIES) {
        delay(WIFI_RETRY_INTERVAL);
        attempts++;
    }

    DynamicJsonDocument res(256);
    res["success"] = WiFi.status() == WL_CONNECTED;
    res["message"] = WiFi.status() == WL_CONNECTED
        ? "Connected to " + String(ssid)
        : "Failed to connect to " + String(ssid);

    String response;
    serializeJson(res, response);
    request->send(200, "application/json", response);
}
