#include "websocket.h"
#include "logger.h"
#include "utils/json_helper.h"

AsyncWebSocket WebSocketServer::ws("/ws");
size_t WebSocketServer::clientCount = 0;

void WebSocketServer::begin(AsyncWebServer& server) {
    ws.onEvent(handleEvent);
    server.addHandler(&ws);
    LOG_INFO("WebSocket server started on /ws");
}

void WebSocketServer::handleEvent(AsyncWebSocket* server, AsyncWebSocketClient* client,
                                   AwsEventType type, void* arg, uint8_t* data, size_t len) {
    switch (type) {
        case WS_EVT_CONNECT:
            clientCount++;
            LOG_INFO("WebSocket client connected (total: %d)", clientCount);
            break;

        case WS_EVT_DISCONNECT:
            if (clientCount > 0) clientCount--;
            LOG_INFO("WebSocket client disconnected (total: %d)", clientCount);
            break;

        case WS_EVT_DATA:
            break;

        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

void WebSocketServer::broadcastEvent(const char* event, const WifiNetwork& network) {
    String payload = JsonHelper::createEventPayload(event, network);
    ws.textAll(payload);
    LOG_DEBUG("Event broadcast: %s - %s", event, network.ssid.c_str());
}
