#include "api_routes.h"
#include "logger.h"
#include "wifi_scanner.h"
#include "utils/json_helper.h"

void ApiRoutes::begin(AsyncWebServer& server) {
    LOG_INFO("Setting up API routes...");
    
    // Health check endpoint
    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        handleRoot(request);
    });
    
    // WiFi API endpoint
    server.on("/api/wifi", HTTP_GET, [](AsyncWebServerRequest* request) {
        handleWifiApi(request);
    });
    
    LOG_INFO("API routes initialized");
}

void ApiRoutes::handleRoot(AsyncWebServerRequest* request) {
    LOG_INFO("GET / (health check)");
    request->send(200, "text/plain", "OK");
}

void ApiRoutes::handleWifiApi(AsyncWebServerRequest* request) {
    LOG_INFO("GET /api/wifi");
    
    const auto& networks = WifiScanner::getNetworks();
    String response = JsonHelper::createWifiResponse(networks);
    
    request->send(200, "application/json", response);
}
