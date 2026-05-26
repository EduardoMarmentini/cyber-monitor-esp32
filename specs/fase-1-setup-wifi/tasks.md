# TASKS — Fase 1: Setup, Configuração & WiFi Scanner

**Status:** ✅ Completo  
**Total Tasks:** 8 principais + subtasks

---

## Task 1: Inicializar Projeto PlatformIO

**Status:** `[x] Completed`  
**Prioridade:** 🔴 CRÍTICA (bloqueia tudo)  
**Estimado:** 2 horas

### Descrição
Criar novo projeto PlatformIO com Arduino Framework para ESP32dev.

### Subtasks
- [x] Criar diretório `/firmware` na raiz do repositório
- [x] Adicionar `platformio.ini` com config ESP32dev
- [x] Adicionar libraries: `me-no-dev/ESP Async WebServer`, `bblanchon/ArduinoJson@^6.19.0`, `AsyncTCP`
- [x] Validar `platformio run` compila sem erros
- [x] Criar diretório structure: `src/`, `include/`, `lib/`, `models/`, `utils/`
- [x] Git commit: `init(platform): platformio setup for esp32`

### Critério de Aceitação
- ✓ `platformio.ini` existe e está válido
- ✓ `platformio run` compila e produz `.elf` file
- ✓ Diretórios criados conforme estrutura esperada

### Notas
- Usar `platform = espressif32`, `board = esp32dev`, `framework = arduino`
- Monitor speed: 115200 baud

---

## Task 2: Implementar Wi-Fi Conexão

**Status:** `[ ] Not Started`  
**Prioridade:** 🔴 CRÍTICA (bloqueia scanner)  
**Estimado:** 3-4 horas

### Descrição
Conectar ESP32 ao WiFi automaticamente na startup.

### Subtasks
- [ ] Criar `src/config.h` com #define WIFI_SSID e WIFI_PASSWORD
- [ ] Implementar função `setupWiFi()` em `src/main.cpp`
- [ ] Usar WiFi.begin(SSID, PASSWORD)
- [ ] Implementar retry logic (max 10 tentativas, wait 500ms entre elas)
- [ ] Log WiFi status: "Connecting...", "Connected!", "Failed"
- [ ] Após conectar, imprimir IP address no serial
- [ ] Git commit: `feat(wifi): wifi connection setup`

### Critério de Aceitação
- ✓ ESP32 conecta à sua rede WiFi automaticamente
- ✓ Serial mostra "Connected! IP: 192.168.x.x"
- ✓ Se SSID inválido, mostra "Connection failed" e tenta reconectar

### Notas
- Usar Serial.begin(115200) no setup()
- WiFi.mode(WIFI_STA) para station mode
- WiFi.waitForConnectResult() retorna WL_CONNECTED quando pronto

---

## Task 3: Implementar WiFi Scanner

**Status:** `[ ] Not Started`  
**Prioridade:** 🔴 CRÍTICA (core feature)  
**Estimado:** 4-5 horas

### Descrição
Escanear redes WiFi próximas periodicamente.

### Subtasks
- [ ] Criar `src/models/wifi_network.h` com struct WifiNetwork
- [ ] Implementar `src/wifi_scanner.h` com classes/funções
- [ ] Implementar `src/wifi_scanner.cpp` com lógica de scan
- [ ] Função `scanWifiNetworks()` chama WiFi.scanNetworks()
- [ ] Extrair: SSID, BSSID, RSSI, channel, encryption
- [ ] Armazenar em `std::vector<WifiNetwork>` ou array
- [ ] Executar scan a cada 10 segundos em loop não-bloqueante
- [ ] Log cada scan: "[INFO] WiFi scan complete: 5 networks found"
- [ ] Git commit: `feat(scanner): wifi network scanner implementation`

### Critério de Aceitação
- ✓ scan compila e executa
- ✓ Serial mostra redes detectadas a cada 10s
- ✓ Output inclui SSID, RSSI, channel, encryption
- ✓ Não bloqueia main loop

### Notas
- WiFi.scanNetworks() é assíncrono internamente mas blocking na call
- Usar WiFi.SSID(i), WiFi.RSSI(i), WiFi.channel(i), WiFi.encryptionType(i)
- RSSI típico: -30 (forte) a -90 (fraco), em dBm
- Encryption: 0=Open, 1=WEP, 2=WPA, 3=WPA2, 4=WPA3

---

## Task 4: Criar HTTP Server & Rotas

**Status:** `[ ] Not Started`  
**Prioridade:** 🔴 CRÍTICA (API core)  
**Estimado:** 3-4 horas

### Descrição
Inicializar servidor HTTP e criar estrutura de rotas.

### Subtasks
- [ ] Criar `src/api_routes.h` com declarações de rotas
- [ ] Implementar `src/api_routes.cpp`
- [ ] Inicializar AsyncWebServer(80) em setup()
- [ ] Criar rota GET `/api/wifi` (vazia por enquanto)
- [ ] Adicionar rota GET `/` health check (simple "OK" response)
- [ ] Server.begin() após todas routes registradas
- [ ] Log startup: "[INFO] HTTP Server started on port 80"
- [ ] Git commit: `feat(api): http server and routes setup`

### Critério de Aceitação
- ✓ HTTP server compila
- ✓ GET http://ESP32_IP/ retorna "OK"
- ✓ GET http://ESP32_IP/api/wifi não dá 404 (mas retorna vazio)
- ✓ Serial confirma server started

### Notas
- Usar AsyncWebServer da biblioteca `me-no-dev/ESP Async WebServer`
- Rotas handler async: `request->send(200, "text/plain", "OK")`
- Port 80 é default HTTP

---

## Task 5: Implementar JSON Helper & Serialization

**Status:** `[ ] Not Started`  
**Prioridade:** 🟡 ALTA (necessário para responses)  
**Estimado:** 3-4 horas

### Descrição
Criar helper functions para serialização JSON padronizada.

### Subtasks
- [ ] Criar `src/utils/json_helper.h`
- [ ] Implementar `src/utils/json_helper.cpp`
- [ ] Função `createWiFiResponse(vector<WifiNetwork>)` retorna JSON string
- [ ] Função `addTimestamp()` adiciona Unix timestamp
- [ ] JSON structure: `{success: bool, timestamp: long, count: int, data: []}`
- [ ] Testar com exemplo hardcoded
- [ ] Git commit: `feat(utils): json serialization helpers`

### Critério de Aceitação
- ✓ JSON output é válido (sem sintaxe erros)
- ✓ Timestamp é Unix time em milissegundos
- ✓ Array `data` contém objetos WifiNetwork

### Notas
- ArduinoJson: usar `JsonDocument` para criar objetos
- `output.printTo(buffer)` para converter em string
- Exemplo estrutura:
  ```json
  {
    "success": true,
    "timestamp": 1740000000,
    "count": 2,
    "data": [{"ssid": "NET", "rssi": -40, ...}]
  }
  ```

---

## Task 6: Implementar Endpoint GET /api/wifi

**Status:** `[ ] Not Started`  
**Prioridade:** 🔴 CRÍTICA (entrega principal)  
**Estimado:** 2-3 horas

### Descrição
Conectar WiFi scanner com JSON helper e servir via HTTP.

### Subtasks
- [ ] Modificar `api_routes.cpp` GET `/api/wifi` handler
- [ ] Handler chama `scanWifiNetworks()` ou usa cached results
- [ ] Serializa resultado com `createWiFiResponse()`
- [ ] Retorna JSON com status 200 OK e content-type application/json
- [ ] Testar com curl: `curl http://ESP32_IP/api/wifi`
- [ ] Git commit: `feat(api): implement GET /api/wifi endpoint`

### Critério de Aceitação
- ✓ GET /api/wifi retorna JSON válido
- ✓ JSON contém todos campos esperados
- ✓ status 200 OK
- ✓ content-type é application/json

### Notas
- Não fazer scan em cada request (pode travar); usar cached results
- Scan executa em background a cada 10s
- Response time deve ser <100ms

---

## Task 7: Implementar Logger Básico

**Status:** `[ ] Not Started`  
**Prioridade:** 🟢 MÉDIA (nice-to-have para fase 1)  
**Estimado:** 2-3 horas

### Descrição
Sistema de logs simples no serial com timestamps.

### Subtasks
- [ ] Criar `src/logger.h` com funções `Logger::info()`, `Logger::warn()`, `Logger::error()`
- [ ] Implementar `src/logger.cpp`
- [ ] Formato: `[HH:MM:SS] [LEVEL] Message`
- [ ] Usar millis() para timestamp relativo
- [ ] Log todos eventos importantes: WiFi, server start, scans, requests
- [ ] Fase 2 expandirá isso com mais features
- [ ] Git commit: `feat(logger): basic serial logging`

### Critério de Aceitação
- ✓ Logs aparecem no serial com formato padronizado
- ✓ INFO, WARN, ERROR levels diferenciados
- ✓ Timestamp relativo funciona

### Notas
- Fase 4 fará logger completo com níveis configuráveis
- Por enquanto: just Serial.print com formato fixo

---

## Task 8: Testing & Cleanup (Fase 1)

**Status:** `[ ] Not Started`  
**Prioridade:** 🟡 ALTA (validação)  
**Estimado:** 2-3 horas

### Descrição
Testar tudo junto, validar checkpoints, cleanup código.

### Subtasks
- [ ] Compilar projeto: `platformio run` (zero warnings)
- [ ] Flash para ESP32: `platformio run --target upload`
- [ ] Observar logs seriais no `platformio device monitor`
- [ ] Verificar WiFi conecta: "Connected! IP: x.x.x.x"
- [ ] Verificar scans executam a cada 10s
- [ ] Testar GET /api/wifi: `curl http://ESP32_IP/api/wifi`
- [ ] Verificar JSON válido: `curl http://ESP32_IP/api/wifi | jq .`
- [ ] Validar todos checkpoints em checklist.md
- [ ] Code cleanup: remover commented code, adicionar comments
- [ ] Git commit: `refactor(fase1): cleanup and final tests`

### Critério de Aceitação
- ✓ Compilação: Zero erros, <5 warnings
- ✓ Flash: Sucesso diretamente
- ✓ Logs: Aparecem no serial, timestamp ok
- ✓ HTTP: GET /api/wifi retorna JSON válido
- ✓ Checklist: 100% checkpoints marcados ✓

### Notas
- Se houver warnings, documentar por que são OK
- Código deve ser legível e comentado
- Nada commitado sem passar em testes

---

## Priorização de Tasks

### 🔴 CRÍTICAS (bloqueadores)
1. Task 1: PlatformIO setup
2. Task 2: WiFi connection
3. Task 3: WiFi scanner
4. Task 4: HTTP server
5. Task 6: GET /api/wifi

### 🟡 ALTAS (necessárias)
6. Task 5: JSON helper
7. Task 8: Testing

### 🟢 MÉDIAS (nice-to-have)
8. Task 7: Logger básico

---

## Progresso Global Fase 1

```
[ ] Task 1: PlatformIO setup              0%
[ ] Task 2: WiFi connection               0%
[ ] Task 3: WiFi scanner                  0%
[ ] Task 4: HTTP server & routes          0%
[ ] Task 5: JSON helpers                  0%
[ ] Task 6: GET /api/wifi endpoint        0%
[ ] Task 7: Logger básico                 0%
[ ] Task 8: Testing & cleanup             0%

TOTAL PROGRESSO FASE 1: 0/8 = 0%
```

---

## Changelog

| Data | Mudança |
|------|---------|
| 22/05/2026 | Criação inicial de tasks Fase 1 |

