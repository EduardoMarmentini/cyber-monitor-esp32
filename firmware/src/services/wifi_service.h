#pragma once

#include <Arduino.h>
#include <vector>
#include <functional>
#include "models/wifi_network.h"

using WifiNetworkCallback = std::function<void(const WifiNetwork& network)>;

class WifiService {
public:
    static void begin();
    static void update();
    static const std::vector<WifiNetwork>& getCachedNetworks() {
        return cachedNetworks;
    }
    static void onNetworkFound(WifiNetworkCallback callback) {
        networkFoundCallback = callback;
    }

private:
    static std::vector<WifiNetwork> cachedNetworks;
    static std::vector<String> knownBssids;
    static WifiNetworkCallback networkFoundCallback;
    static bool initialized;
};
