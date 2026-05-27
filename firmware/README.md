# ESP32 Cyber Monitor

Firmware para ESP32 que escaneia redes WiFi e dispositivos BLE, expondo dados via API REST e WebSocket em tempo real.

## Funcionalidades

- Scanner WiFi (SSID, BSSID, RSSI, canal, criptografia)
- Scanner BLE (nome, MAC, RSSI, UUID)
- API REST com endpoints JSON
- WebSocket para eventos em tempo real
- Cache de dispositivos (evita duplicatas)
- Configuração dinâmica de WiFi via API (persiste em NVS)
- Watchdog com auto-reset
- Logger configurável com níveis

## Endpoints

| Rota | Método | Descrição |
|------|--------|-----------|
| `/` | GET | Health check |
| `/api/wifi` | GET | Lista redes WiFi detectadas |
| `/api/ble` | GET | Lista dispositivos BLE detectados |
| `/api/stats` | GET | Estatísticas do dispositivo |
| `/api/config/wifi` | GET | SSID atual configurado |
| `/api/config/wifi` | POST | Altera SSID/senha WiFi |
| `/ws` | WebSocket | Eventos em tempo real |

## Exemplos de Uso

### Listar redes WiFi
```bash
curl http://esp32-ip/api/wifi | jq .
```

### Listar dispositivos BLE
```bash
curl http://esp32-ip/api/ble | jq .
```

### Estatísticas
```bash
curl http://esp32-ip/api/stats | jq .
```

### Configurar WiFi dinamicamente
```bash
curl -X POST http://esp32-ip/api/config/wifi \
  -H "Content-Type: application/json" \
  -d '{"ssid": "MinhaRede", "password": "minha_senha"}'
```

### WebSocket (eventos em tempo real)
```bash
wscat -c ws://esp32-ip/ws
```

## Eventos WebSocket

### WIFI_UPDATE
```json
{
  "event": "WIFI_UPDATE",
  "timestamp": 1740000000,
  "payload": {
    "ssid": "NETGEAR",
    "bssid": "00:11:22:33:44:55",
    "rssi": -42,
    "channel": 6,
    "encryption": "WPA2"
  }
}
```

### BLE_UPDATE
```json
{
  "event": "BLE_UPDATE",
  "timestamp": 1740000000,
  "payload": {
    "name": "Galaxy Buds",
    "mac": "11:22:33:44:55:66",
    "rssi": -61,
    "uuid": "180D"
  }
}
```

## Build e Flash

```bash
# Compilar
platformio run

# Flash no ESP32
platformio run --target upload

# Monitor serial
platformio device monitor
```

## Configuração

Edite `src/config.h` para definir:

- `WIFI_SSID` / `WIFI_PASSWORD` — rede padrão (usada apenas se NVS vazia)
- `API_PORT` — porta do servidor HTTP
- `WIFI_SCAN_INTERVAL` — intervalo entre scans WiFi (ms)
- `BLE_SCAN_INTERVAL` — intervalo entre scans BLE (ms)
- `WATCHDOG_TIMEOUT_SEC` — timeout do watchdog (segundos)

Após o primeiro boot, a configuração WiFi pode ser alterada via API e persiste na NVS.

## Estrutura do Projeto

```
firmware/
├── platformio.ini
├── README.md
└── src/
    ├── main.cpp
    ├── config.h
    ├── logger.h/cpp
    ├── wifi_scanner.h/cpp
    ├── ble_scanner.h/cpp
    ├── api_routes.h/cpp
    ├── websocket.h/cpp
    ├── models/
    │   ├── wifi_network.h
    │   ├── ble_device.h
    │   └── stats.h
    ├── services/
    │   ├── wifi_service.h/cpp
    │   ├── ble_service.h/cpp
    │   └── stats_service.h/cpp
    └── utils/
        └── json_helper.h/cpp
```

## Requisitos

- ESP32 Dev Kit
- PlatformIO (VS Code extension ou CLI)
- Cabo USB para programação
