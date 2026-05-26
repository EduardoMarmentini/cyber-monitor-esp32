#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "models/wifi_network.h"
#include "models/ble_device.h"

class WebSocketServer {
public:
    static void begin(AsyncWebServer& server);
    static void broadcastEvent(const char* event, const WifiNetwork& network);
    static void broadcastBleEvent(const char* event, const BleDevice& device);
    static size_t getClientCount() {
        return clientCount;
    }

private:
    static AsyncWebSocket ws;
    static size_t clientCount;

    static void handleEvent(AsyncWebSocket* server, AsyncWebSocketClient* client,
                            AwsEventType type, void* arg, uint8_t* data, size_t len);
};
