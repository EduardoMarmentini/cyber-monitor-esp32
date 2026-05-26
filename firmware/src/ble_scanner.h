#pragma once

#include <Arduino.h>
#include <vector>
#include "models/ble_device.h"

class BleScanner {
public:
    // Initialize BLE scanner
    static void init();
    
    // Perform BLE scan
    // Returns vector of discovered BLE devices
    // Duration: how long to scan in seconds
    static std::vector<BleDevice> scan(int durationSeconds = 10);
    
    // Get name of device
    static String getDeviceNameFromAddress(String address);

private:
    static bool initialized;
};
