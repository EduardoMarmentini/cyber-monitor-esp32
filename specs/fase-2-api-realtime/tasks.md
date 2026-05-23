# TASKS — Fase 2: API REST Completa & WebSocket Real-time

**Status:** ⏳ Bloqueada por Fase 1  
**Total Tasks:** 6 principais

---

## Task 1: Implementar WiFi Service & Caching

**Status:** `[ ] Not Started`  
**Prioridade:** 🔴 CRÍTICA  
**Estimado:** 4-5 horas

### Descrição
Criar camada de serviço para WiFi com cache inteligente.

### Subtasks
- [ ] Criar `src/services/wifi_service.h` com classe WifiService
- [ ] Implementar `src/services/wifi_service.cpp`
- [ ] Armazenar último scan em variável static/global
- [ ] Comparar novo scan com cache
- [ ] Detectar redes novas (não no cache anterior)
- [ ] Detectar redes que desapareceram
- [ ] Emitir callback para novas redes encontradas
- [ ] Git commit: `feat(service): wifi service with caching`

### Critério de Aceitação
- ✓ WifiService::getInstance().scan() retorna lista de redes
- ✓ Cache evita enviar eventos duplicados
- ✓ Registro de mudanças (novo, removido, RSSI changed)

### Notas
- Usar singleton pattern para WifiService
- Cache comparado por BSSID (MAC address é único)

---

## Task 2: Implementar GET /api/stats Endpoint

**Status:** `[ ] Not Started`  
**Prioridade:** 🔴 CRÍTICA  
**Estimado:** 2-3 horas

### Descrição
Endpoint que retorna estatísticas globais do dispositivo.

### Subtasks
- [ ] Criar `src/services/stats_service.h/cpp` (opcional, pode ficar inline)
- [ ] Implementar função `getStats()` que retorna struct Stats
- [ ] Contar WiFi networks (do scanner)
- [ ] Contar BLE devices (will be 0 in Fase 2, filled in Fase 3)
- [ ] Calcular uptime com millis() / 1000
- [ ] Modificar `src/api_routes.cpp` para GET /api/stats
- [ ] Retornar JSON: {success, data: {wifiNetworks, bleDevices, uptime}}
- [ ] Git commit: `feat(api): implement GET /api/stats endpoint`

### Critério de Aceitação
- ✓ GET /api/stats retorna JSON válido
- ✓ wifiNetworks é número > 0
- ✓ bleDevices é número (0 em Fase 2)
- ✓ uptime aumenta a cada segundo

### Notas
- Uptime em segundos (millis() / 1000)
- BLE será preenchido em Fase 3

---

## Task 3: Implementar WebSocket Server

**Status:** `[ ] Not Started`  
**Prioridade:** 🔴 CRÍTICA  
**Estimado:** 5-6 horas

### Descrição
Configurar servidor WebSocket para comunicação real-time.

### Subtasks
- [ ] Adicionar dependência `WebSocketsServer` (ou usar AsyncWebServer + WebSocket handler)
- [ ] Criar `src/websocket.h` com classe WebSocketServer
- [ ] Implementar `src/websocket.cpp`
- [ ] Inicializar servidor WS na porta 81 (ou mesmo que HTTP com rota /ws)
- [ ] Função para broadcast message para todos clientes
- [ ] Função para handle client connect
- [ ] Função para handle client disconnect
- [ ] Função para handle incoming message
- [ ] Modificar `src/main.cpp` para inicializar WS no setup()
- [ ] Adicionar chamada `handleWebSocket()` no loop principal
- [ ] Log: "[INFO] WebSocket server started on ws://IP:81"
- [ ] Log: "[INFO] Client connected (total: N)" quando novo cliente
- [ ] Git commit: `feat(websocket): websocket server implementation`

### Critério de Aceitação
- ✓ WebSocket server inicia sem erro
- ✓ Clientes conseguem conectar com `ws://ESP32_IP:81` ou `ws://ESP32_IP/ws`
- ✓ Server não trava com múltiplos clientes (20+ conexões teste)
- ✓ Disconnect gracioso sem memory leak

### Notas
- Usar `ESPAsyncWebServer` com WebSocket support (mais simples)
- Alternativamente: biblioteca `WebSocketsServer` standalone
- Para Fase 2: apenas structure, sem eventos ainda (vir em Task 4)

---

## Task 4: Emitir Eventos WIFI_UPDATE

**Status:** `[ ] Not Started`  
**Prioridade:** 🔴 CRÍTICA  
**Estimado:** 3-4 horas

### Descrição
Quando nova rede WiFi detectada, broadcast evento para todos clientes.

### Subtasks
- [ ] Modificar WiFi scanner para chamar `onWifiNetworkFound()` callback
- [ ] Implementar `onWifiNetworkFound(WifiNetwork)` em `websocket.cpp`
- [ ] Callback serializa rede em JSON: `{event: "WIFI_UPDATE", payload: {ssid, rssi}}`
- [ ] Broadcast para todos clientes WS conectados
- [ ] Log: "[INFO] WIFI_UPDATE event sent (SSID: NETGEAR, RSSI: -45)"
- [ ] Testar com cliente WS simples (wscat ou página HTML)
- [ ] Git commit: `feat(events): broadcast WIFI_UPDATE events`

### Critério de Aceitação
- ✓ Novo cliente conecta ao WS
- ✓ Quando rede WiFi adicionada, cliente recebe JSON event
- ✓ Event contém: event="WIFI_UPDATE", payload com ssid e rssi
- ✓ Múltiplos clientes recebem evento simultâneos

### Notas
- Evento por SSID novo, não por cada scan
- Usar cache comparação (Task 1) para detectar "novo"

---

## Task 5: Expandir Logger (Multiple Levels)

**Status:** `[ ] Not Started`  
**Prioridade:** 🟡 ALTA  
**Estimado:** 2-3 horas

### Descrição
Expandir Logger básico (Fase 1) com suporte a níveis múltiplos.

### Subtasks
- [ ] Modificar `src/logger.h`
- [ ] Adicionar níveis: INFO, WARN, ERROR, DEBUG
- [ ] Adicionar funções: `Logger::info()`, `Logger::warn()`, `Logger::error()`, `Logger::debug()`
- [ ] Implementar formato: `[HH:MM:SS] [LEVEL] Message`
- [ ] Adicionar variável global para min log level (permite desativar DEBUG)
- [ ] Usar printf-style strings: `Logger::info("WiFi: %s", ssid.c_str())`
- [ ] Modificar `src/logger.cpp` com implementação
- [ ] Usar em todos módulos ao invés de Serial.print direto
- [ ] Git commit: `feat(logger): multi-level logging system`

### Critério de Aceitação
- ✓ Logs aparecem no serial com prefixes [INFO], [WARN], [ERROR]
- ✓ DEBUG logs podem ser desativados (opcional)
- ✓ Todos eventos importantes logados (WiFi, WS, requests, etc)

### Notas
- Formato timestamp com millis(): `[MM:SS.ms]` é suficiente
- Fase 4 expandirá para SPIFFS logging

---

## Task 6: Implementar Configuração Dinâmica de WiFi (NVS)

**Status:** `[ ] Not Started`  
**Prioridade:** 🟡 ALTA  
**Estimado:** 4-5 horas

### Descrição
Permitir alterar SSID/senha WiFi via API, persistindo em NVS (Preferences.h) para não precisar recompilar ao mudar de rede.

### Subtasks
- [ ] Adicionar `#include <Preferences.h>` para acesso à NVS
- [ ] Criar função `loadWifiConfig()` em `main.cpp` que lê da NVS (fallback para config.h)
- [ ] Criar função `saveWifiConfig(ssid, password)` que escreve na NVS
- [ ] Implementar `POST /api/config/wifi` em `api_routes.cpp`
- [ ] Validar body JSON: `{ "ssid": "...", "password": "..." }` (password opcional)
- [ ] Salvar na NVS e tentar conectar à nova rede
- [ ] Retornar `{ "success": true, "message": "Conectado à nova rede" }` ou erro
- [ ] Implementar `GET /api/config/wifi` — retorna SSID atual (sem senha)
- [ ] Modificar `main.cpp` setupWiFi() para usar config da NVS primeiro
- [ ] Log: "[INFO] WiFi config loaded from NVS: SSID" ou "[INFO] Using default config"
- [ ] Git commit: `feat(config): dynamic wifi config via API with NVS persistence`

### Critério de Aceitação
- ✓ POST /api/config/wifi com SSID/senha válidos conecta e persiste
- ✓ POST /api/config/wifi com SSID aberto (sem senha) funciona
- ✓ Após reboot, ESP32 conecta na rede salva na NVS
- ✓ GET /api/config/wifi retorna SSID sem expor senha
- ✓ Se NVS vazia, usa fallback de config.h

### Notas
- Usar `Preferences.h` (já incluso no Arduino ESP32) — namespace "wifi", key "ssid" e "password"
- `preferences.begin("wifi", false)` para leitura/escrita
- Para rede aberta, password pode ser `""` (string vazia) ou omitido

---

## Task 7: Testing & Cleanup (Fase 2)

**Status:** `[ ] Not Started`  
**Prioridade:** 🟡 ALTA  
**Estimado:** 2-3 horas

### Descrição
Validar tudo junto e marcar Fase 2 como completa.

### Subtasks
- [ ] Compilar: `platformio run` (zero warnings)
- [ ] Flash: `platformio run --target upload`
- [ ] Verificar todos endpoints HTTP funcionam
- [ ] Conectar cliente WebSocket: `wscat -c ws://ESP32_IP:81`
- [ ] Gerar evento WIFI_UPDATE (por exemplo, movendo router perto)
- [ ] Verificar cliente WS recebe evento JSON
- [ ] Testar múltiplos clientes WS simultâneos
- [ ] Verificar memory não aumenta indefinidamente (no memory leak)
- [ ] Validar todos checkpoints em `checklist.md`
- [ ] Code cleanup: remover dead code, adicionar comments
- [ ] Git commit: `refactor(fase2): cleanup and final tests`

### Critério de Aceitação
- ✓ Compilação: Zero erros, <5 warnings
- ✓ HTTP: Todos 3 endpoints funcionam
- ✓ WebSocket: Clientes conectam, eventos fluem
- ✓ Checklist: 100% checkpoints marcados

---

## Priorização de Tasks

### 🔴 CRÍTICAS
1. Task 1: WiFi Service & Caching
2. Task 2: GET /api/stats
3. Task 3: WebSocket Server
4. Task 4: WIFI_UPDATE Events

### 🟡 ALTAS
5. Task 5: Logger Multiple Levels
6. Task 6: WiFi Config Dinâmica (NVS)
7. Task 7: Testing & Cleanup

---

## Progresso Global Fase 2

```
[ ] Task 1: WiFi Service & Caching        0%
[ ] Task 2: GET /api/stats                0%
[ ] Task 3: WebSocket Server              0%
[ ] Task 4: WIFI_UPDATE Events            0%
[ ] Task 5: Logger Multiple Levels        0%
[ ] Task 6: WiFi Config Dinâmica (NVS)    0%
[ ] Task 7: Testing & Cleanup             0%

TOTAL PROGRESSO FASE 2: 0/7 = 0%
```

---

## Changelog

| Data | Mudança |
|------|---------|
| 22/05/2026 | Criação inicial de tasks Fase 2 |

