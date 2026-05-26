#pragma once

#include <ESPAsyncWebServer.h>

class ApiRoutes {
public:
    static void begin(AsyncWebServer& server);

private:
    static void handleRoot(AsyncWebServerRequest* request);
    static void handleWifiApi(AsyncWebServerRequest* request);
    static void handleStatsApi(AsyncWebServerRequest* request);
    static void handleConfigWifiGet(AsyncWebServerRequest* request);
    static void handleConfigWifiPost(AsyncWebServerRequest* request);
};
