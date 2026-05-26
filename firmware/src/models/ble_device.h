#pragma once

#include <Arduino.h>

struct BleDevice {
    String name;
    String mac;
    int rssi;
    String uuid;

    // For caching/comparison
    bool operator==(const BleDevice& other) const {
        return mac == other.mac;
    }
};
