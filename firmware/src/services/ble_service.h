#pragma once

#include <Arduino.h>
#include <vector>
#include "models/ble_device.h"

// Callback for BLE device events
typedef void (*BleDeviceCallback)(const BleDevice& device);

class BleService {
public:
    // Singleton pattern
    static BleService& getInstance();

    // Initialize the service
    void init();

    // Perform scan and return current devices
    std::vector<BleDevice> scan();

    // Set callback for new devices
    void onNewDevice(BleDeviceCallback callback);

    // Get cached devices
    const std::vector<BleDevice>& getCachedDevices() const;

    // Get count of cached devices
    int getDeviceCount() const;

private:
    BleService() = default;
    ~BleService() = default;

    // Prevent copy
    BleService(const BleService&) = delete;
    BleService& operator=(const BleService&) = delete;

    // Cache of last scan results
    std::vector<BleDevice> cachedDevices;

    // Callback for new devices
    BleDeviceCallback newDeviceCallback = nullptr;

    // Compare scan results with cache to detect new/removed devices
    void updateCache(const std::vector<BleDevice>& scanResults);
};
