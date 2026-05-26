#include "ble_service.h"
#include "logger.h"
#include "ble_scanner.h"

BleService& BleService::getInstance() {
    static BleService instance;
    return instance;
}

void BleService::init() {
    LOG_INFO("Initializing BleService");
    BleScanner::init();
    LOG_INFO("BleService initialized");
}

std::vector<BleDevice> BleService::scan() {
    LOG_DEBUG("BleService: Starting scan...");

    // Perform actual BLE scan
    std::vector<BleDevice> scanResults = BleScanner::scan(10);

    // Update cache and detect new devices
    updateCache(scanResults);

    return scanResults;
}

void BleService::onNewDevice(BleDeviceCallback callback) {
    newDeviceCallback = callback;
}

const std::vector<BleDevice>& BleService::getCachedDevices() const {
    return cachedDevices;
}

int BleService::getDeviceCount() const {
    return cachedDevices.size();
}

void BleService::updateCache(const std::vector<BleDevice>& scanResults) {
    // Detect NEW devices (in scanResults but not in cache)
    for (const auto& newDevice : scanResults) {
        bool found = false;
        for (const auto& cachedDevice : cachedDevices) {
            if (newDevice.mac == cachedDevice.mac) {
                found = true;
                break;
            }
        }

        // New device detected
        if (!found) {
            LOG_INFO("New BLE device detected: %s (%s) RSSI: %d",
                     newDevice.name.c_str(),
                     newDevice.mac.c_str(),
                     newDevice.rssi);

            // Call callback if set
            if (newDeviceCallback != nullptr) {
                newDeviceCallback(newDevice);
            }
        }
    }

    // Detect REMOVED devices (in cache but not in scanResults)
    std::vector<String> removedMacs;
    for (const auto& cachedDevice : cachedDevices) {
        bool found = false;
        for (const auto& scanDevice : scanResults) {
            if (cachedDevice.mac == scanDevice.mac) {
                found = true;
                break;
            }
        }

        if (!found) {
            LOG_INFO("BLE device removed: %s (%s)", cachedDevice.name.c_str(), cachedDevice.mac.c_str());
            removedMacs.push_back(cachedDevice.mac);
        }
    }

    // Remove devices no longer found
    for (const auto& mac : removedMacs) {
        auto it = cachedDevices.begin();
        while (it != cachedDevices.end()) {
            if (it->mac == mac) {
                it = cachedDevices.erase(it);
            } else {
                ++it;
            }
        }
    }

    // Update cache with current results
    cachedDevices = scanResults;

    LOG_DEBUG("BLE cache updated: %d devices", cachedDevices.size());
}
