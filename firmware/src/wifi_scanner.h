#pragma once

#include <Arduino.h>
#include <vector>
#include "models/wifi_network.h"

class WifiScanner {
public:
    // Initialize WiFi scanner
    static void begin();
    
    // Perform WiFi scan
    static void scan();
    
    // Get last scan results
    static const std::vector<WifiNetwork>& getNetworks() {
        return networks;
    }
    
    // Check if scan is in progress
    static bool isScanning() {
        return scanning;
    }

private:
    static std::vector<WifiNetwork> networks;
    static bool scanning;
    static unsigned long lastScanTime;
    
    static void printNetworks();
};
