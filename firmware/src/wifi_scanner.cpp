#include <WiFi.h>
#include "wifi_scanner.h"
#include "logger.h"

std::vector<WifiNetwork> WifiScanner::networks;
bool WifiScanner::scanning = false;
unsigned long WifiScanner::lastScanTime = 0;
ScanCallback WifiScanner::scanCallback = nullptr;

void WifiScanner::begin() {
    LOG_INFO("WiFi Scanner initialized");

    // melhora estabilidade do scan
    WiFi.mode(WIFI_STA);
    WiFi.disconnect(false, false);

    // potência máxima
    WiFi.setTxPower(WIFI_POWER_19_5dBm);
}

void WifiScanner::scan() {
    if (scanning) {
        return;
    }

    scanning = true;

    LOG_DEBUG("Starting WiFi scan...");

    // scan síncrono mais estável
    int n = WiFi.scanNetworks(
        false, // async
        true   // hidden networks
    );

    networks.clear();

    if (n <= 0) {
        LOG_WARN("No WiFi networks found");
    } else {
        LOG_INFO("%d WiFi networks found", n);

        for (int i = 0; i < n; i++) {

            // ignora sinal muito fraco
            if (WiFi.RSSI(i) < -95) {
                continue;
            }

            WifiNetwork network;

            String ssid = WiFi.SSID(i);

            if (ssid.length() == 0) {
                ssid = "Rede Oculta";
            }

            network.ssid = ssid;
            network.bssid = WiFi.BSSIDstr(i);
            network.rssi = WiFi.RSSI(i);
            network.channel = WiFi.channel(i);

            wifi_auth_mode_t security = WiFi.encryptionType(i);

            switch (security) {
                case WIFI_AUTH_OPEN:
                    network.encryption = "Open";
                    break;

                case WIFI_AUTH_WEP:
                    network.encryption = "WEP";
                    break;

                case WIFI_AUTH_WPA_PSK:
                    network.encryption = "WPA";
                    break;

                case WIFI_AUTH_WPA2_PSK:
                    network.encryption = "WPA2";
                    break;

                case WIFI_AUTH_WPA_WPA2_PSK:
                    network.encryption = "WPA/WPA2";
                    break;

                case WIFI_AUTH_WPA2_ENTERPRISE:
                    network.encryption = "WPA2 Enterprise";
                    break;

                case WIFI_AUTH_WPA3_PSK:
                    network.encryption = "WPA3";
                    break;

                default:
                    network.encryption = "Unknown";
                    break;
            }

            networks.push_back(network);

            LOG_DEBUG(
                "SSID: %s | RSSI: %d | CH: %d | ENC: %s",
                network.ssid.c_str(),
                network.rssi,
                network.channel,
                network.encryption.c_str()
            );
        }
    }

    WiFi.scanDelete();

    lastScanTime = millis();
    scanning = false;

    if (scanCallback) {
        scanCallback(networks);
    }
}

void WifiScanner::printNetworks() {
    for (const auto& network : networks) {
        LOG_DEBUG(
            "SSID: %s | RSSI: %d dBm | Channel: %d | Encryption: %s",
            network.ssid.c_str(),
            network.rssi,
            network.channel,
            network.encryption.c_str()
        );
    }
}