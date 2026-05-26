#include "wifi_service.h"
#include "logger.h"
#include "wifi_scanner.h"
#include <WiFi.h>

std::vector<WifiNetwork> WifiService::cachedNetworks;
std::vector<String> WifiService::knownBssids;
WifiNetworkCallback WifiService::networkFoundCallback = nullptr;
bool WifiService::initialized = false;

void WifiService::begin() {
    LOG_INFO("WiFi Service initialized");
    initialized = true;
}

void WifiService::update() {
    if (!initialized) return;

    const auto& scannedNetworks = WifiScanner::getNetworks();

    for (const auto& network : scannedNetworks) {
        bool isNew = true;
        for (const auto& bssid : knownBssids) {
            if (bssid == network.bssid) {
                isNew = false;
                break;
            }
        }

        if (isNew) {
            knownBssids.push_back(network.bssid);
            cachedNetworks.push_back(network);
            LOG_DEBUG("New network detected: %s (%s)", network.ssid.c_str(), network.bssid.c_str());

            if (networkFoundCallback) {
                networkFoundCallback(network);
            }
        }
    }
}
