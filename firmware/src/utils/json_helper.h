#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <vector>
#include "models/wifi_network.h"
#include "models/ble_device.h"

class JsonHelper {
public:
    static String createWifiResponse(const std::vector<WifiNetwork>& networks);
    static String createBleResponse(const std::vector<BleDevice>& devices);
    static String createStatsResponse(int wifiCount, int bleCount, unsigned long uptime);
    static String createEventPayload(const char* event, const WifiNetwork& network);
    static String createBleEventPayload(const char* event, const BleDevice& device);

private:
    static String getEncryptionType(int encryptionType);
};
