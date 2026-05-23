# CONFIG — Fase 3: BLE Scanner & Integração

**Última atualização:** 22/05/2026

---

## Configurações Adicionadas (config.h)

```cpp
// BLE Configuration (novo em Fase 3)
#define BLE_SCAN_INTERVAL 15000           // BLE scan a cada 15 segundos
#define BLE_SCAN_DURATION 10              // Duração do BLE scan em segundos
#define BLE_DEVICE_CACHE_SIZE 32          // Max dispositivos armazenados

// Scan Timing (offset para evitar contenção)
#define BLE_SCAN_OFFSET 5000              // Offset de 5s (BLE = 10s + 5s, WiFi = 10s)

// BLE Advertisement Types (optional)
#define BLE_MIN_RSSI -100                 // RSSI mínimo para considerar válido

// Stats Service (expandido para incluir BLE)
#define ENABLE_BLE_STATS 1                // Ativar contagem de devices BLE
```

---

## Estrutura de Diretórios Atualizada

```
/firmware/
├── src/
│   ├── main.cpp                         (modificado: init BLE)
│   ├── config.h                         (expandido)
│   ├── ble_scanner.h                    (novo)
│   ├── ble_scanner.cpp                  (novo)
│   ├── wifi_scanner.h/cpp               (mantém)
│   ├── api_routes.h/cpp                 (modificado: +/api/ble)
│   ├── websocket.h/cpp                  (modificado: +BLE_UPDATE)
│   ├── models/
│   │   ├── wifi_network.h               (mantém)
│   │   ├── ble_device.h                 (novo)
│   │   └── stats.h                      (mantém)
│   ├── services/
│   │   ├── wifi_service.h/cpp           (mantém)
│   │   ├── ble_service.h                (novo)
│   │   ├── ble_service.cpp              (novo)
│   │   └── stats_service.h/cpp          (modificado: +bleDevices)
│   └── utils/
│       ├── json_helper.h/cpp            (mantém)
├── platformio.ini                        (mantém)
└── README.md                             (modificado: +BLE info)
```

---

## Dependências (platformio.ini)

Nenhuma biblioteca nova. BLE usa SDK built-in do ESP32:

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200

lib_deps =
    me-no-dev/ESP Async WebServer
    bblanchon/ArduinoJson@^6.19.0
    AsyncTCP

# Nota: BLEDevice.h é built-in no Arduino core para ESP32
```

---

## Struct BleDevice

```cpp
// models/ble_device.h
struct BleDevice {
    String name;                // Device name (pode estar vazio)
    String mac;                 // MAC address em formato XX:XX:XX:XX:XX:XX
    int rssi;                   // RSSI em dBm (negativo)
    String uuid;                // Service UUID (opcional)
};
```

---

## Estrutura JSON: BLE Events

### BLE_UPDATE Event
```json
{
  "event": "BLE_UPDATE",
  "timestamp": 1740000000123,
  "payload": {
    "name": "Galaxy Buds",
    "mac": "11:22:33:44:55:66",
    "rssi": -61,
    "uuid": "180D"
  }
}
```

### GET /api/ble Response
```json
{
  "success": true,
  "timestamp": 1740000000123,
  "count": 3,
  "data": [
    {
      "name": "Galaxy Buds",
      "mac": "11:22:33:44:55:66",
      "rssi": -61,
      "uuid": "180D"
    },
    {
      "name": "Mi Band 5",
      "mac": "AA:BB:CC:DD:EE:FF",
      "rssi": -75,
      "uuid": "180A"
    }
  ]
}
```

---

## Limites & Performance (Fase 3)

| Item | Limite | Nota |
|------|--------|------|
| BLE devices máximo | 32 | Configurável |
| WiFi scan interval | 10s | Mantém de Fase 2 |
| BLE scan interval | 15s | Novo em Fase 3 |
| Scan offset | 5s | Evita contenção |
| Cache size | ~40KB | Por tipo |
| Free RAM requerido | >150KB | Após init (aumentado) |

---

## Scan Timing Diagram

```
Timeline (segundos):
0s    - WiFi scan inicia
2s    - WiFi scan completa
5s    - BLE scan inicia
7s    - BLE scan completa
10s   - WiFi scan inicia (2º ciclo)
12s   - WiFi scan completa
15s   - BLE scan inicia (2º ciclo)
...
```

**Intervalo:** WiFi a cada 10s, BLE a cada 15s com 5s offset.

---

## Memory Layout (Fase 3)

Estimativa de RAM after initialization:

```
- WiFi scanner + cache: ~40KB
- BLE scanner + cache: ~45KB (novo)
- WebSocket buffer: ~20KB
- WS client sessions: ~5KB per client
- Logger, JSON temp: ~15KB
- Free: ~100KB (diminuído, mas ainda acceptable)
```

**Total: ~350KB de 512KB disponíveis**

Monitor com `ESP.getFreeHeap()` regularmente.

---

## BLE Scanner Implementation Notes

### BLE GAP Scan (usamos isso)
- Scan advertisement packets
- **NÃO** conecta aos devices
- **NÃO** lê GATT characteristics
- Apenas detecção passiva

### Exemplo API
```cpp
BLEDevice::init("ESP32_Cyber_Monitor");
BLEScan* pBLEScan = BLEDevice::getScan();
pBLEScan->setActiveScan(false);  // Passive scan
pBLEScan->setInterval(100);
pBLEScan->setWindow(99);
BLEScanResults* foundDevices = pBLEScan->start(10);  // 10 segundos
```

### Data Coletado
- **Device Address:** MAC address único
- **Device Name:** Advertising name (pode ser vazio)
- **RSSI:** Signal strength
- **Service UUID:** Se oferecido no advertisement

---

## Troubleshooting BLE (Fase 3)

### BLE scan não encontra devices
- Verificar se há BLE devices próximos
- Testar com smartphone próximo
- Validar se BLE antena não está danificada

### Memory cresce com scans
- Validar se cleanup feito após scan
- Procurar em vector erase
- Checar se callback não aloca infinito

### Contenção WiFi/BLE
- Manter 5s offset entre scans
- Se ainda trava, aumentar delay
- Usar `vTaskDelay()` para dar tempo ao FreeRTOS

### Duplicatas em events
- MAC address comparison correta
- Validar cache não deletando antigos

---

## Build & Flash Commands (Fase 3)

```bash
# Build
cd /firmware
platformio run

# Flash + Monitor
platformio run --target upload && platformio device monitor
```

---

## Serial Monitor Expected Output (Fase 3)

```
[00:00:15] [INFO] System starting...
[00:00:16] [INFO] Connecting to WiFi...
[00:00:20] [INFO] WiFi connected! IP: 192.168.1.100
[00:00:20] [INFO] HTTP server started on port 80
[00:00:20] [INFO] WebSocket server started
[00:00:20] [INFO] BLE scanner initialized
[00:00:20] [INFO] Starting scan loops...
[00:00:10] [INFO] WiFi scan complete: 5 networks found
[00:00:15] [INFO] BLE scan complete: 3 devices found
[00:00:20] [INFO] WiFi scan complete: 5 networks found
[00:00:25] [INFO] BLE_UPDATE event sent (Device: Galaxy Buds, RSSI: -61)
[00:00:30] [INFO] WiFi scan complete: 5 networks found
[00:00:35] [INFO] BLE scan complete: 3 devices found
...
```

---

## Verification Commands

### Test HTTP Endpoints
```bash
curl http://192.168.1.100/api/wifi
curl http://192.168.1.100/api/ble
curl http://192.168.1.100/api/stats
```

### Test WebSocket Events
```bash
wscat -c ws://192.168.1.100:81
# Aguardar WIFI_UPDATE e BLE_UPDATE events em JSON
```

### Monitor Free RAM
```cpp
Serial.printf("Free RAM: %d bytes\n", ESP.getFreeHeap());
```

---

## Changelog

| Data | Mudança |
|------|---------|
| 22/05/2026 | Criação inicial de CONFIG Fase 3 |

