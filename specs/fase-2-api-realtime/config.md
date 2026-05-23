# CONFIG — Fase 2: API REST Completa & WebSocket Real-time

**Última atualização:** 22/05/2026

---

## Configurações Adicionadas (config.h)

```cpp
// WebSocket Configuration (novo em Fase 2)
#define WS_PORT 81                        // WebSocket port
#define WS_ENDPOINT "/ws"                 // WebSocket path (Alt: .on("/ws", ...))
#define MAX_WS_CLIENTS 10                 // Máximo de clientes simultâneos

// Logger Configuration (expandido em Fase 2)
#define LOG_LEVEL_DEBUG
#define LOG_LEVEL_INFO
#define LOG_LEVEL_WARN
#define LOG_LEVEL_ERROR
// #define DISABLE_DEBUG_LOGS              // Descomente para desabilitar DEBUG

// WiFi Service (cache)
#define WIFI_CACHE_UPDATE_INTERVAL 5000   // Check cache a cada 5s

// Stats Service
#define STATS_UPDATE_INTERVAL 1000        // Update stats a cada 1s

// NVS WiFi Config (runtime)
#define NVS_NAMESPACE "wifi"              // Namespace na NVS para WiFi config
#define NVS_KEY_SSID "ssid"               // Key para SSID
#define NVS_KEY_PASSWORD "password"       // Key para password
```

---

## Estrutura de Diretórios Atualizada

```
/firmware/
├── src/
│   ├── main.cpp                         (modificado: init WebSocket)
│   ├── config.h                         (expandido)
│   ├── logger.h/cpp                     (expandido: multi-level)
│   ├── api_routes.h/cpp                 (modificado: +/api/stats, +/api/config/wifi)
│   ├── wifi_scanner.h/cpp               (mantém)
│   ├── websocket.h                      (novo)
│   ├── websocket.cpp                    (novo)
│   ├── models/
│   │   ├── wifi_network.h               (mantém)
│   │   └── stats.h                      (novo)
│   ├── services/
│   │   ├── wifi_service.h               (novo)
│   │   ├── wifi_service.cpp             (novo)
│   │   └── stats_service.h/cpp          (novo)
│   └── utils/
│       ├── json_helper.h/cpp            (mantém)
├── platformio.ini                        (mantém)
└── README.md                             (modificado: +WebSocket docs)
```

---

## Dependências Adicionais (platformio.ini)

Nenhuma biblioteca nova requerida. As bibliotecas de Fase 1 já suportam WebSocket:
- `ESPAsyncWebServer` já tem suporte nativo para WebSocket
- Alternativa: adicionar `WebSocketsServer` se necessário compilação manual

```ini
lib_deps =
    me-no-dev/ESP Async WebServer        (já suporta WebSocket)
    bblanchon/ArduinoJson@^6.19.0
    AsyncTCP
```

---

## Estrutura JSON: Eventos WebSocket

### WIFI_UPDATE Event
```json
{
  "event": "WIFI_UPDATE",
  "timestamp": 1740000000123,
  "payload": {
    "ssid": "NETGEAR",
    "bssid": "00:11:22:33:44:55",
    "rssi": -42,
    "channel": 6,
    "encryption": "WPA2"
  }
}
```

### Stats Update (opcional, pode ser on-demand)
```json
{
  "event": "STATS_UPDATE",
  "timestamp": 1740000000123,
  "payload": {
    "wifiNetworks": 5,
    "bleDevices": 0,
    "uptime": 120
  }
}
```

---

## Limites & Performance

| Item | Limite | Nota |
|------|--------|------|
| WebSocket clients máximo | 10 | Configurável em config.h |
| Event queue size | 50 | Buffer de eventos |
| Message size max | 2KB | Para JSON event |
| Cache de redes | ~20 | Típico em ambiente |
| Free RAM requerido | >150KB | Após init |

---

## WebSocket Client Example (HTML)

```html
<!DOCTYPE html>
<html>
<head><title>ESP32 Monitor</title></head>
<body>
<h1>WiFi Monitor</h1>
<ul id="events"></ul>

<script>
const ws = new WebSocket('ws://192.168.1.100:81');

ws.onopen = () => {
  console.log('Connected to ESP32');
  const li = document.createElement('li');
  li.textContent = 'Connected';
  document.getElementById('events').appendChild(li);
};

ws.onmessage = (event) => {
  console.log('Received:', event.data);
  const msg = JSON.parse(event.data);
  const li = document.createElement('li');
  li.textContent = `${msg.event}: ${msg.payload.ssid}`;
  document.getElementById('events').appendChild(li);
};

ws.onerror = (error) => {
  console.error('WebSocket error:', error);
};
</script>
</body>
</html>
```

---

## Logger Format & Output

### Serial Output Examples

#### INFO
```
[00:15:23] [INFO] WiFi connected! IP: 192.168.1.100
[00:15:24] [INFO] WebSocket server started
[00:15:34] [INFO] WiFi scan complete: 5 networks found
```

#### WARN
```
[00:20:45] [WARN] WiFi signal weak (-80 dBm)
[00:20:50] [WARN] WebSocket buffer full, dropping messages
```

#### ERROR
```
[00:30:00] [ERROR] WiFi connection lost!
[00:30:05] [ERROR] Failed to broadcast event
```

#### DEBUG (pode ser desativado)
```
[00:05:10] [DEBUG] Cache compared: 0 new, 0 removed
[00:05:15] [DEBUG] WS broadcast to 2 clients
```

---

## Memory Layout (Fase 2)

Estimativa de RAM usage após init:

```
- WiFi scanner + cache: ~40KB
- WebSocket buffer: ~20KB
- Session objects (WS clients): ~5KB per client
- Logger buffer: ~5KB
- JSON serialization temp: ~10KB
- Free: ~150KB (ideal)
```

**Total aproximado: ~300KB de 512KB disponíveis**

---

## Timeouts & Intervals

| Configure | Valor | Propósito |
|-----------|-------|----------|
| WiFi scan interval | 10s | Frequência de scan |
| Cache update check | 5s | Comparar cache |
| Stats update | 1s | Refresh counters |
| WebSocket client timeout | 30s | Disconnect idle |
| HTTP request timeout | 5s | AsyncWebServer default |

---

## Monitoring & Debug Commands

### Check Free RAM
```cpp
Serial.printf("Free RAM: %d bytes\n", ESP.getFreeHeap());
ESP.getChipRevision();
ESP.getSdkVersion();
```

### WebSocket Clients Connected
```cpp
Serial.printf("Active WS clients: %d\n", wsServer.count());
```

### Log Level Control
Modificar em runtime (opcional):
```cpp
Logger::setLevel(LogLevel::DEBUG);  // Habilitar DEBUG
Logger::setLevel(LogLevel::WARN);   // Apenas WARN+
```

---

## Build & Test Commands

### Build Fase 2
```bash
cd /firmware
platformio run
```

### Flash & Monitor
```bash
platformio run --target upload && platformio device monitor
```

### Test HTTP endpoints
```bash
curl http://192.168.1.100/api/wifi
curl http://192.168.1.100/api/stats
```

### Test WiFi Config (NVS)
```bash
# Ver SSID atual
curl http://192.168.1.100/api/config/wifi

# Alterar rede (com senha)
curl -X POST http://192.168.1.100/api/config/wifi \
  -H "Content-Type: application/json" \
  -d '{"ssid":"NovaRede","password":"senha"}'

# Rede aberta (sem senha)
curl -X POST http://192.168.1.100/api/config/wifi \
  -H "Content-Type: application/json" \
  -d '{"ssid":"RedeAberta"}'
```

### Test WebSocket
```bash
npm install -g wscat   # Instalar wscat
wscat -c ws://192.168.1.100:81
> send {"test": "data"}
```

---

## Troubleshooting Fase 2

### WebSocket não conecta
- Verificar se server iniciou: log "[INFO] WebSocket server started"
- Verificar port (81 ou /ws endpoint correto)
- Testar com browser DevTools ou wscat

### Memory leak em WebSocket
- Verificar se cliente disconnect é loggado
- Checar se broadcast função não aloca indefinidamente
- Use `ESP.getFreeHeap()` para monitorar

### Eventos não chegam
- Validar estrutura JSON
- Checar se cache detectou "novo" (no duplicado)
- Testar manual: `Logger::info("Broadcasting event")`

### Timeout HTTP/WebSocket
- Aumentar timeout em AsyncWebServer config
- Validar não há operação blocking no loop

---

## Changelog

| Data | Mudança |
|------|---------|
| 22/05/2026 | Criação inicial de CONFIG Fase 2 |

