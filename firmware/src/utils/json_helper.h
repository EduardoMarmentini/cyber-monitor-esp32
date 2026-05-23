#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <vector>
#include "models/wifi_network.h"

class JsonHelper {
public:
    // Creates JSON response for WiFi networks
    static String createWifiResponse(const std::vector<WifiNetwork>& networks);
    
private:
    static String getEncryptionType(int encryptionType);
};
