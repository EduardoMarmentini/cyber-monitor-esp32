#include "stats_service.h"
#include "wifi_scanner.h"

unsigned long StatsService::startTime = 0;

void StatsService::begin() {
    startTime = millis();
}

Stats StatsService::getStats() {
    Stats stats;
    stats.wifiNetworks = WifiScanner::getNetworks().size();
    stats.bleDevices = 0;
    stats.uptime = (millis() - startTime) / 1000;
    return stats;
}
