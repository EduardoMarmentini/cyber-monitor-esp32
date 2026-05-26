#include "json_helper.h"

String JsonHelper::createWifiResponse(const std::vector<WifiNetwork>& networks) {
    DynamicJsonDocument doc(4096);

    doc["success"] = true;
    doc["timestamp"] = millis();
    doc["count"] = networks.size();

    JsonArray dataArray = doc.createNestedArray("data");

    for (const auto& network : networks) {
        JsonObject obj = dataArray.createNestedObject();
        obj["ssid"] = network.ssid;
        obj["bssid"] = network.bssid;
        obj["rssi"] = network.rssi;
        obj["channel"] = network.channel;
        obj["encryption"] = network.encryption;
    }

    String output;
    serializeJson(doc, output);
    return output;
}

String JsonHelper::createBleResponse(const std::vector<BleDevice>& devices) {
    DynamicJsonDocument doc(4096);

    doc["success"] = true;
    doc["timestamp"] = millis();
    doc["count"] = devices.size();

    JsonArray dataArray = doc.createNestedArray("data");

    for (const auto& device : devices) {
        JsonObject obj = dataArray.createNestedObject();
        obj["name"] = device.name;
        obj["mac"] = device.mac;
        obj["rssi"] = device.rssi;
        obj["uuid"] = device.uuid;
    }

    String output;
    serializeJson(doc, output);
    return output;
}

String JsonHelper::createStatsResponse(int wifiCount, int bleCount, unsigned long uptime) {
    DynamicJsonDocument doc(256);

    doc["success"] = true;
    JsonObject data = doc.createNestedObject("data");
    data["wifiNetworks"] = wifiCount;
    data["bleDevices"] = bleCount;
    data["uptime"] = uptime;

    String output;
    serializeJson(doc, output);
    return output;
}

String JsonHelper::createEventPayload(const char* event, const WifiNetwork& network) {
    DynamicJsonDocument doc(512);

    doc["event"] = event;
    doc["timestamp"] = millis();
    JsonObject payload = doc.createNestedObject("payload");
    payload["ssid"] = network.ssid;
    payload["bssid"] = network.bssid;
    payload["rssi"] = network.rssi;
    payload["channel"] = network.channel;
    payload["encryption"] = network.encryption;

    String output;
    serializeJson(doc, output);
    return output;
}

String JsonHelper::createBleEventPayload(const char* event, const BleDevice& device) {
    DynamicJsonDocument doc(512);

    doc["event"] = event;
    doc["timestamp"] = millis();
    JsonObject payload = doc.createNestedObject("payload");
    payload["name"] = device.name;
    payload["mac"] = device.mac;
    payload["rssi"] = device.rssi;
    payload["uuid"] = device.uuid;

    String output;
    serializeJson(doc, output);
    return output;
}

String JsonHelper::getEncryptionType(int encryptionType) {
    switch (encryptionType) {
        case 0: return "Open";
        case 1: return "WEP";
        case 2: return "WPA";
        case 3: return "WPA2";
        case 4: return "WPA3";
        default: return "Unknown";
    }
}
