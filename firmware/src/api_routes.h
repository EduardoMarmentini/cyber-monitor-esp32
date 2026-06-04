#pragma once

#include <ESPAsyncWebServer.h>

class ApiRoutes {
public:
    static void begin(AsyncWebServer& server);

private:
    static void handleConect(AsyncWebServerRequest* request);
    static void handleWifiApi(AsyncWebServerRequest* request);
};
