#pragma once

#include <Arduino.h>
#include <vector>
#include <functional>
#include "models/wifi_network.h"

using ScanCallback = std::function<void(const std::vector<WifiNetwork>&)>;

class WifiScanner {
public:
    static void begin();
    static void scan();
    static const std::vector<WifiNetwork>& getNetworks() {
        return networks;
    }
    static bool isScanning() {
        return scanning;
    }
    static void onScanComplete(ScanCallback callback) {
        scanCallback = callback;
    }

private:
    static std::vector<WifiNetwork> networks;
    static bool scanning;
    static unsigned long lastScanTime;
    static ScanCallback scanCallback;
    static void printNetworks();
};
