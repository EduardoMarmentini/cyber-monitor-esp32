#include "ble_scanner.h"
#include "logger.h"
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

bool BleScanner::initialized = false;

static std::vector<BleDevice> scannedDevices;

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
public:
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        BleDevice device;

        if (advertisedDevice.haveName()) {
            device.name = String(advertisedDevice.getName().c_str());
        } else {
            device.name = "Unknown";
        }

        device.mac = advertisedDevice.getAddress().toString().c_str();
        device.rssi = advertisedDevice.getRSSI();

        if (advertisedDevice.haveServiceUUID()) {
            device.uuid = String(advertisedDevice.getServiceUUID().toString().c_str());
        } else {
            device.uuid = "N/A";
        }

        bool found = false;
        for (const auto& dev : scannedDevices) {
            if (dev.mac == device.mac) {
                found = true;
                break;
            }
        }

        if (!found) {
            scannedDevices.push_back(device);
        }
    }
};

void BleScanner::init() {
    if (initialized) {
        LOG_DEBUG("BLE already initialized");
        return;
    }

    LOG_INFO("Initializing BLE...");
    try {
        BLEDevice::init("ESP32_Cyber_Monitor");
        initialized = true;
        LOG_INFO("BLE initialized successfully");
    } catch (const std::exception& e) {
        LOG_ERROR("BLE initialization failed: %s", e.what());
        initialized = false;
    }
}

std::vector<BleDevice> BleScanner::scan(int durationSeconds) {
    if (!initialized) {
        LOG_ERROR("BLE not initialized");
        return std::vector<BleDevice>();
    }

    if (durationSeconds < 1) durationSeconds = 1;
    if (durationSeconds > 30) durationSeconds = 30;

    LOG_INFO("Starting BLE scan for %d seconds...", durationSeconds);
    scannedDevices.clear();

    try {
        BLEScan* pBLEScan = BLEDevice::getScan();
        if (pBLEScan == nullptr) {
            LOG_ERROR("Failed to get BLE scan object");
            return std::vector<BleDevice>();
        }

        // Use static callback to avoid memory leak
        static MyAdvertisedDeviceCallbacks callbacks;
        pBLEScan->setAdvertisedDeviceCallbacks(&callbacks);
        pBLEScan->setActiveScan(true);
        pBLEScan->setInterval(100);
        pBLEScan->setWindow(99);

        BLEScanResults scanResults = pBLEScan->start(durationSeconds, false);
        pBLEScan->clearResults();

        LOG_INFO("BLE scan complete: %d devices found", scannedDevices.size());
    } catch (const std::exception& e) {
        LOG_ERROR("BLE scan failed: %s", e.what());
    }

    return scannedDevices;
}

String BleScanner::getDeviceNameFromAddress(String address) {
    for (const auto& device : scannedDevices) {
        if (device.mac == address) {
            return device.name;
        }
    }
    return "Unknown";
}
