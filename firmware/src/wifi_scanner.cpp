#include <WiFi.h>
#include "wifi_scanner.h"
#include "logger.h"

std::vector<WifiNetwork> WifiScanner::networks;
bool WifiScanner::scanning = false;
unsigned long WifiScanner::lastScanTime = 0;

void WifiScanner::begin() {
    LOG_INFO("WiFi Scanner initialized");
}

void WifiScanner::scan() {
    if (scanning) {
        return;  // Already scanning
    }
    
    scanning = true;
    LOG_INFO("WiFi scan started");
    
    // Perform the scan
    int n = WiFi.scanNetworks();
    
    networks.clear();
    
    if (n == 0) {
        LOG_WARN("No WiFi networks found");
    } else {
        LOG_INFO("WiFi scan complete: %d networks found", n);
        
        for (int i = 0; i < n; i++) {
            WifiNetwork network;
            network.ssid = WiFi.SSID(i);
            network.bssid = WiFi.BSSIDstr(i);
            network.rssi = WiFi.RSSI(i);
            network.channel = WiFi.channel(i);
            
            // Map encryption type
            uint8_t security = WiFi.encryptionType(i);
            switch (security) {
                case 0: network.encryption = "Open"; break;
                case 1: network.encryption = "WEP"; break;
                case 2: network.encryption = "WPA"; break;
                case 3: network.encryption = "WPA2"; break;
                case 4: network.encryption = "WPA3"; break;
                default: network.encryption = "Unknown"; break;
            }
            
            networks.push_back(network);
        }
        
        printNetworks();
    }
    
    WiFi.scanDelete();
    lastScanTime = millis();
    scanning = false;
}

void WifiScanner::printNetworks() {
    for (const auto& network : networks) {
        LOG_INFO("  SSID: %s | RSSI: %d dBm | Channel: %d | Encryption: %s",
                 network.ssid.c_str(), network.rssi, network.channel, network.encryption.c_str());
    }
}
