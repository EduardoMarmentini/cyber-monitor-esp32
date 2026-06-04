#include "api_routes.h"
#include "logger.h"
#include "wifi_scanner.h"
#include "utils/json_helper.h"
#include <ArduinoJson.h>

static void sendCors(AsyncWebServerRequest* request, int code, const String& contentType, const String& content) {
    AsyncWebServerResponse *response = request->beginResponse(code, contentType, content);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}

void ApiRoutes::begin(AsyncWebServer& server) {
    LOG_INFO("Setting up API routes...");

    server.on("/conect", HTTP_GET, [](AsyncWebServerRequest* request) {
        handleConect(request);
    });

    server.on("/api/wifi", HTTP_GET, [](AsyncWebServerRequest* request) {
        handleWifiApi(request);
    });

    LOG_INFO("API routes initialized");
}

void ApiRoutes::handleConect(AsyncWebServerRequest* request) {
    LOG_DEBUG("GET /conect");

    DynamicJsonDocument doc(128);
    doc["status"] = "ok";
    doc["message"] = "API is online";

    String response;
    serializeJson(doc, response);
    sendCors(request, 200, "application/json", response);
}

void ApiRoutes::handleWifiApi(AsyncWebServerRequest* request) {
    LOG_DEBUG("GET /api/wifi");

    const auto& networks = WifiScanner::getNetworks();
    String response = JsonHelper::createWifiResponse(networks);

    sendCors(request, 200, "application/json", response);
}
