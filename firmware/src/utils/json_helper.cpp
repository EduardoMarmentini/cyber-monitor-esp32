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
