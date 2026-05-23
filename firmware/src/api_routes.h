#pragma once

#include <ESPAsyncWebServer.h>

class ApiRoutes {
public:
    // Initialize API routes
    static void begin(AsyncWebServer& server);
    
private:
    // Handlers
    static void handleRoot(AsyncWebServerRequest* request);
    static void handleWifiApi(AsyncWebServerRequest* request);
};
