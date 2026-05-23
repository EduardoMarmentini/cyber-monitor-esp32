# CONFIG — Fase 1: Setup, Configuração & WiFi Scanner

**Última atualização:** 22/05/2026

---

## Configurações Locais (config.h)

```cpp
// WiFi Credentials
#define WIFI_SSID "SUA_REDE_AQUI"        // SSID da sua rede WiFi
#define WIFI_PASSWORD "SUA_SENHA_AQUI"   // Senha do WiFi

// API Configuration
#define API_PORT 80                       // Porta HTTP (padrão 80)

// Scanner Timings
#define WIFI_SCAN_INTERVAL 10000          // WiFi scan a cada 10 segundos (ms)

// Serial Configuration
#define SERIAL_BAUD_RATE 115200           // Velocidade do monitor serial

// WiFi Configuration
#define WIFI_CONNECT_TIMEOUT 10000        // Timeout de conexão Wi-Fi (10s)
#define WIFI_CONNECT_RETRIES 10           // Número de tentativas de conexão
#define WIFI_RETRY_INTERVAL 500           // Intervalo entre tentativas (ms)
```

---

## Configurações PlatformIO (platformio.ini)

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
monitor_port = /dev/ttyUSB0              ; Linux (alterar para COM3 no Windows)

; Library dependencies
lib_deps =
    me-no-dev/ESP Async WebServer
    bblanchon/ArduinoJson@^6.19.0
    AsyncTCP

; Build flags
build_flags =
    -D CONFIG_FREERTOS_ENABLE_REBOOT_ON_PANIC

; Upload settings
upload_speed = 921600

; Advanced settings
board_build.f_cpu = 240000000L            ; CPU frequency
```

---

## Estrutura de Diretórios Esperada

```
/firmware/
├── src/
│   ├── main.cpp                         ← Entry point
│   ├── config.h                         ← Configurações globais
│   ├── wifi_scanner.h                   ← Declarações
│   ├── wifi_scanner.cpp                 ← Implementação
│   ├── api_routes.h
│   ├── api_routes.cpp
│   ├── logger.h
│   ├── logger.cpp
│   ├── models/
│   │   └── wifi_network.h               ← Struct de dados WiFi
│   └── utils/
│       ├── json_helper.h
│       └── json_helper.cpp
├── include/                              ← Headers do projeto (opcional)
├── lib/                                  ← Bibliotecas customizadas
├── platformio.ini                        ← Configuração de build
├── README.md                             ← Documentação
└── .gitignore                            ← Ignore build artifacts
```

---

## Variáveis de Ambiente

Nenhuma variável de ambiente especial necessária para Fase 1.

---

## Limites & Restrições

| Item | Limite | Nota |
|------|--------|------|
| WiFi networks máximo | ~20 | Típico de um ambiente |
| JSON response size | <4KB | Cabe fácil em RAM do ESP32 |
| Scan duration | ~1-2s | Bloqueante durante scan |
| HTTP request timeout | 5s | Default AsyncWebServer |
| Free RAM mínimo | >100KB | Para operação estável |

---

## Hardware Requirements

### Core
- **ESP32 Dev Kit** (recomendado para desenvolviamento)
  - RAM: 512KB SRAM + 4MB Flash
  - WiFi: 802.11 b/g/n integrado
  - BLE: Integrado (para usar em Fase 3)

### Conectividade
- **Cabo USB Micro-B** (para programação)
- **Rede WiFi** com SSID/password conhecidos
- **Antena WiFi** (integrada ou externa)

### Ferramentas
- **VS Code** com **PlatformIO Extension**
- **Python 3.x** (required by PlatformIO)
- **esptool.py** (included with PlatformIO)

---

## Bibliotecas Requeridas (versões específicas)

| Library | Versão | Propósito | Link |
|---------|--------|----------|------|
| ESP Async WebServer | latest | HTTP server não-bloqueante | https://github.com/me-no-dev/ESPAsyncWebServer |
| ArduinoJson | 6.19.0+ | JSON serialization | https://github.com/bblanchon/ArduinoJson |
| AsyncTCP | latest | Suporte TCP async | https://github.com/me-no-dev/AsyncTCP |
| WiFi.h | built-in | WiFi driver (Arduino core) | N/A |

**Instalação automática:** `platformio run` fará download automático conforme `platformio.ini`

---

## Serial Monitor Configuration

Conectar ao ESP32 via:

```bash
# Linux/Mac
platformio device monitor --port /dev/ttyUSB0 --baud 115200

# Windows
platformio device monitor --port COM3 --baud 115200

# Simples (alias)
pio device monitor
```

**Atalhos no monitor:**
- `Ctrl+]` para sair
- `Ctrl+T` para menu de opções

---

## Memory Mapping (ESP32)

Não há preocupações especiais para Fase 1, mas validar:
- Código: <1MB (plenty of margin)
- Data: <50KB (scanner + logger)
- Heap: >100KB free após setup

**Verificar em runtime:**
```cpp
Serial.printf("Free RAM: %d bytes\n", ESP.getFreeHeap());
```

---

## Debug & Logging Levels

### Nível INFO
```
[12:34:56] [INFO] WiFi connected! IP: 192.168.1.100
[12:34:57] [INFO] HTTP server started on port 80
[12:35:07] [INFO] WiFi scan complete: 5 networks found
```

### Nível WARN
```
[12:35:30] [WARN] WiFi signal weak (-75 dBm)
[12:35:45] [WARN] HTTP request timeout (no response)
```

### Nível ERROR
```
[12:36:00] [ERROR] WiFi connection lost!
[12:36:05] [ERROR] Failed to initialize HTTP server
```

---

## Network Configuration

### ESP32 Station (STA) Mode
- Conecta a um WiFi router existente
- Modo utilizando nesta fase
- Recebe IP dinamicamente via DHCP

### Constantes WiFi
```cpp
WiFi.mode(WIFI_STA);           // Station mode
WiFi.setSleep(false);          // Desabilitar sleep (opcional)
WiFi.setAutoConnect(true);     // Reconectar automaticamente
```

---

## Performance Targets (Fase 1)

| Métrica | Target | Aceitável |
|---------|--------|-----------|
| Boot time | <5s | <10s |
| WiFi connect | <10s | <30s |
| Scan duration | 1-2s | <5s |
| API response time | <100ms | <500ms |
| Memory usage | <200KB | <300KB |
| Free RAM | >100KB | >50KB |

---

## Build & Flash Commands

### Build só (sem upload)
```bash
platformio run
```

### Build + Flash para ESP32
```bash
platformio run --target upload
```

### Monitor serial
```bash
platformio device monitor
```

### Build + Flash + Monitor (one-liner)
```bash
platformio run --target upload && platformio device monitor
```

### Clean build
```bash
platformio run --target clean
platformio run
```

---

## Troubleshooting Configuração

### Issue: Port não reconhecido
**Solução:** Verificar driver USB (CH340 ou FTDI)

### Issue: Compilação falha
**Solução:** `platformio update` ou limpar `.pio/`

### Issue: WiFi não conecta
**Solução:** Verificar SSID/password em `config.h`, signal strength local

### Issue: Serial não aparece
**Solução:** Verificar port em `platformio.ini`, baud rate 115200

### Issue: Restart loop infinito
**Solução:** Memory overflow, remover functions ou reduzir buffer sizes

---

## Changelog

| Data | Mudança |
|------|---------|
| 22/05/2026 | Criação inicial de CONFIG Fase 1 |

