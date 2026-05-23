#pragma once

#include <Arduino.h>

struct WifiNetwork {
    String ssid;
    String bssid;
    int rssi;
    int channel;
    String encryption;
};
