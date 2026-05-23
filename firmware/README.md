# Cyber Monitor ESP32 — Firmware

ESP32 firmware for WiFi and BLE scanning with REST API and WebSocket real-time updates.

## Features (Fase 1)

- ✓ Automatic WiFi connection
- ✓ WiFi network scanner (periodic scans)
- ✓ REST API endpoint `/api/wifi` returning JSON
- ✓ Serial logging with timestamps (current phase)

## Hardware Requirements

- **ESP32 Dev Kit** (or compatible)
- **USB Micro-B cable** for programming
- **WiFi network** with known SSID and password

## Installation

### Prerequisites

1. Install VS Code: https://code.visualstudio.com/
2. Install PlatformIO Extension in VS Code
3. Clone this repository

### Setup

```bash
cd firmware
# Edit config.h with your WiFi SSID and password
nano src/config.h
```

Update these lines:
```cpp
#define WIFI_SSID "YOUR_NETWORK_NAME"
#define WIFI_PASSWORD "YOUR_PASSWORD"
```

## Build & Flash

### Build only
```bash
platformio run
```

### Build and Flash to ESP32
```bash
platformio run --target upload
```

### Monitor Serial Output
```bash
platformio device monitor
```

### All in one (Build + Flash + Monitor)
```bash
platformio run --target upload && platformio device monitor
```

## Usage

### Serial Monitor Output (Expected)

```
[00:00:01] [INFO] System starting...
[00:00:02] [INFO] Connecting to WiFi: YOUR_NETWORK
[00:00:05] [INFO] WiFi connected! IP: 192.168.1.100
[00:00:05] [INFO] WiFi Scanner initialized
[00:00:05] [INFO] Setting up API routes...
[00:00:05] [INFO] HTTP Server started
[00:00:10] [INFO] WiFi scan started
[00:00:11] [INFO] WiFi scan complete: 5 networks found
[00:00:11] [INFO]   SSID: NETGEAR | RSSI: -42 dBm | Channel: 6 | Encryption: WPA2
```

### API Endpoints

#### Health Check
```bash
curl http://192.168.1.100/
# Response: OK
```

#### WiFi Networks
```bash
curl http://192.168.1.100/api/wifi
```

Response:
```json
{
  "success": true,
  "timestamp": 1740000000123,
  "count": 3,
  "data": [
    {
      "ssid": "NETGEAR",
      "bssid": "00:11:22:33:44:55",
      "rssi": -42,
      "channel": 6,
      "encryption": "WPA2"
    }
  ]
}
```

## Troubleshooting

### Port Not Recognized
- Check USB driver (CH340 or FTDI)
- Try different USB cable
- Update platformio.ini `monitor_port`

### WiFi Connection Failed
- Verify SSID and password in `config.h`
- Check signal strength (move ESP32 closer to router)
- Verify network doesn't have hidden SSID (currently not supported)

### No Serial Output
- Verify baud rate is 115200 in `config.h`
- Check monitor_port in `platformio.ini`
- Try: `platformio device monitor --port /dev/ttyUSB0 --baud 115200`

### Compilation Fails
```bash
platformio run clean
platformio run
```

## Project Structure

```
firmware/
├── src/
│   ├── main.cpp                  # Entry point and main loop
│   ├── config.h                  # Configuration constants
│   ├── logger.h/cpp              # Serial logging
│   ├── wifi_scanner.h/cpp        # WiFi scanning logic
│   ├── api_routes.h/cpp          # HTTP route handlers
│   ├── models/
│   │   └── wifi_network.h        # WifiNetwork struct
│   └── utils/
│       └── json_helper.h/cpp     # JSON serialization
├── include/                       # Project headers (if needed)
├── lib/                           # Local libraries
├── platformio.ini                 # Build configuration
└── README.md                      # This file
```

## Next Phase

Phase 2 will add:
- WebSocket server for real-time events
- Complete REST API with stats endpoint
- Enhanced logging system
- WiFi network caching

## References

- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)
- [PlatformIO Docs](https://docs.platformio.org/)
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- [ArduinoJson](https://arduinojson.org/)
