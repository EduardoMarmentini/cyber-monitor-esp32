# CONFIG — Fase 4: Robustez, Otimizações & Finalização

**Última atualização:** 22/05/2026

---

## Configurações Adicionadas (config.h)

```cpp
// Logger Configuration (expandido em Fase 4)
#define DEFAULT_LOG_LEVEL LOG_LEVEL_INFO  // Nível padrão
#define ENABLE_DEBUG_LOGS 1               // Descomente para ativar DEBUG
#define LOG_BUFFER_SIZE 1024              // Buffer para logs (bytes)
#define USE_PROGMEM_STRINGS 1             // Usar F() macros (economia RAM)

// Watchdog Configuration (novo em Fase 4)
#define WATCHDOG_TIMEOUT_SEC 10           // Timeout de 10 segundos
#define WATCHDOG_ENABLE 1                 // Ativar watchdog

// Performance Tuning (opcional)
#define CPU_FREQUENCY_MHZ 240             // CPU at 240 MHz (default)
// #define CPU_FREQUENCY_MHZ 160           // Reduzir para 160 MHz (economia energia)

// Memory Optimization
#define ENABLE_STRING_INTERNING 1         // Cache strings comuns
#define BUFFER_SIZE_SMALL 64              // Buffers pequenos
#define BUFFER_SIZE_MEDIUM 256            // Buffers médios
#define BUFFER_SIZE_LARGE 1024            // Buffers grandes

// Error Recovery
#define ENABLE_AUTO_RECOVERY 1            // Auto-retry em falhas
#define RETRY_COUNT_WIFI 5                // Tentativas WiFi
#define RETRY_COUNT_BLE 3                 // Tentativas BLE
#define TIMEOUT_WIFI_SCAN 15000           // Timeout WiFi (ms)
#define TIMEOUT_BLE_SCAN 12000            // Timeout BLE (ms)
```

---

## Estrutura de Diretórios (Fase 4 — Sem mudanças)

Todas pastas mantidas de Fase 3:

```
/firmware/
├── src/
│   ├── main.cpp                   (modificado: watchdog setup)
│   ├── config.h                   (expandido: logger, watchdog, perf)
│   ├── logger.h/cpp               (significativamente expandido)
│   ├── [todos outros arquivos]    (refactor: error handling, comments)
├── platformio.ini                  (mantém)
├── README.md                        (significativamente expandido)
└── ARCHITECTURE.md                 (novo - opcional)
```

---

## Logger Implementation

### Header Example (src/logger.h)

```cpp
#ifndef LOGGER_H
#define LOGGER_H

enum LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARN = 2,
    ERROR = 3,
    CRITICAL = 4
};

class Logger {
public:
    static void init();
    static void setLevel(LogLevel level);
    
    static void debug(const char* format, ...);
    static void info(const char* format, ...);
    static void warn(const char* format, ...);
    static void error(const char* format, ...);
    static void critical(const char* format, ...);
    
private:
    static LogLevel currentLevel;
    static void log(LogLevel level, const char* format, ...);
};

#endif
```

### Usage Example

```cpp
#include "logger.h"

void setup() {
    Serial.begin(115200);
    Logger::init();
    Logger::setLevel(DEBUG);  // or INFO, WARN, etc
}

void loop() {
    Logger::info("WiFi status: %d", WiFi.status());
    Logger::warn("Signal weak: %d dBm", rssi);
    Logger::error("Connection lost!");
    Logger::debug("Cache size: %d devices", deviceCount);
}
```

---

## Watchdog Implementation

### Setup Example (src/main.cpp)

```cpp
#include "logger.h"
#include <esp_task_wdt.h>

#define WATCHDOG_TIMEOUT_SEC 10

void setupWatchdog() {
    esp_task_wdt_init(WATCHDOG_TIMEOUT_SEC, true);
    esp_task_wdt_add(NULL);
    Logger::info("Watchdog initialized (timeout: %ds)", WATCHDOG_TIMEOUT_SEC);
}

void loop() {
    // ... main code ...
    
    // Feed watchdog (keep it alive)
    esp_task_wdt_reset();
}
```

---

## Memory Layout Final (Fase 4)

Estimativa com otimizações aplicadas:

```
- Code (PROGMEM): ~350KB
- RAM after init:
  - WiFi scanner + cache: ~35KB
  - BLE scanner + cache: ~40KB
  - WebSocket buffer: ~15KB
  - WS clients (5x5KB): ~25KB
  - Logger + buffers: ~10KB
  - Misc (JSON, temp): ~15KB
  - FREE: >150KB (goal)

Total: ~512KB RAM (ESP32 capacity)
```

---

## Performance Targets (Final)

| Métrica | Target | Acceptable |
|---------|--------|-----------|
| Boot time | <5s | <10s |
| API response | <100ms | <200ms |
| WS broadcast | <50ms per client | <100ms |
| Memory stable | >50KB free | >30KB free |
| Watchdog timeout | 10s | 20s max |
| CPU usage | <50% avg | <80% |
| Stability (30 min) | 0 crashes | 0 crashes |

---

## Build Optimization Flags

```ini
[env:esp32dev-optimized]
platform = espressif32
board = esp32dev
framework = arduino

build_flags =
    -O2
    -DNDEBUG
    -DCONFIG_FREERTOS_ENABLE_REBOOT_ON_PANIC
    -ffunction-sections
    -fdata-sections

board_build.f_cpu = 240000000L
```

---

## String Optimization (F() Macros)

### Before (RAM waste)
```cpp
Serial.println("Connected to WiFi");    // String na RAM
Logger::info("WiFi connected");         // String na RAM
```

### After (PROGMEM, otimizado)
```cpp
Serial.println(F("Connected to WiFi")); // String em PROGMEM
Logger::info(F("WiFi connected"));      // String em PROGMEM
```

**Impacto:** ~20-30KB de RAM economizado

---

## README.md Outline (Fase 4)

```markdown
# Cyber Monitor ESP32 Firmware

## Overview
Brief description of project

## Features
- WiFi scanning
- BLE detection
- REST API
- WebSocket real-time
- Robust logging
- Watchdog protection

## Hardware Requirements
- ESP32 Dev Kit
- USB cable
- Antena WiFi integrada

## Installation
- Clone repo
- Install PlatformIO
- Build: platformio run
- Flash: platformio run --target upload

## Usage
### Building
### Flashing
### Serial Monitor Output
### API Endpoints (examples)
### WebSocket Events

## Architecture
(Diagrams/explanation)

## Troubleshooting
(Common issues)

## Performance
(Memory, latency, stability)

## Future Enhancements
(OTA, MQTT, etc)
```

---

## Testing Protocol (Before Release)

### 1. Functional Testing
- [ ] Todos endpoints são acessíveis
- [ ] Todos eventos fluem correto

### 2. Stability Testing
- [ ] 30-min soak test (zero crashes)
- [ ] Memory stable
- [ ] Watchdog não dispara em operação normal

### 3. Performance Testing
- [ ] Response time < 100ms
- [ ] Memory footprint measured
- [ ] CPU usage < 50% average

### 4. Error Handling
- [ ] Kill WiFi → system recovers (sem crash)
- [ ] Remove BLE devices → updates
- [ ] Disconnect WS → clean recovery

### 5. Documentation Review
- [ ] README acionável (alguém consegue buildar)
- [ ] Código comentado adequadamente
- [ ] Git history limpo

---

## Release Checklist

```bash
# Final compilation
platformio run

# Size analysis
size .pio/build/esp32dev/firmware.elf

# Create release tag
git tag -a v1.0-release -m "Cyber Monitor Firmware v1.0"
git push origin v1.0-release

# Create GitHub release
# (include README, build instructions)
```

---

## Deployment Notes

- Backup `config.h` SSID/password antes de compartilhar código
- Considerar usar environment variables para secrets
- Manter histórico Git limpo para produção

---

## Troubleshooting Fase 4

### Logger não mostra DEBUG
- Verificar `Logger::setLevel(DEBUG)` foi chamado
- Compilar com `#define ENABLE_DEBUG_LOGS`

### Watchdog dispara em operação normal
- Aumentar timeout em config.h
- Checar se não há operação bloqueante > 10s

### Memory cresce com tempo
- Procurar string allocation em loop
- Validar vector erase/cleanup
- Use `ESP.getFreeHeap()` para debug

### Performance degradação
- Check if DEBUG logs desativado
- Verificar se não há operação O(n) desnecessária
- Profile com serial output

---

## Changelog

| Data | Mudança |
|------|---------|
| 22/05/2026 | Criação inicial de CONFIG Fase 4 |

