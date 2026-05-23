# TASKS — Fase 3: BLE Scanner & Integração

**Status:** ⏳ Bloqueada por Fases 1-2  
**Total Tasks:** 5 principais

---

## Task 1: Implementar BLE Scanner Básico

**Status:** `[ ] Not Started`  
**Prioridade:** 🔴 CRÍTICA  
**Estimado:** 4-5 horas

### Descrição
Criar scanner BLE que detecta dispositivos próximos.

### Subtasks
- [ ] Criar `src/ble_scanner.h` com funções BLE
- [ ] Implementar `src/ble_scanner.cpp`
- [ ] Incluir `BLEDevice.h` do ESP32 SDK
- [ ] Inicializar BLE: `BLEDevice::init("ESP32_Cyber_Monitor")`
- [ ] Implementar BLE scan: `BLEDevice::getScan()`
- [ ] Duração de scan: 10 segundos (configurável)
- [ ] Extrair para cada device: name, MAC address, RSSI, UUID
- [ ] Armazenar em `std::vector<BleDevice>`
- [ ] Log: "[INFO] BLE scan complete: N devices found"
- [ ] Git commit: `feat(scanner): ble scanner implementation`

### Critério de Aceitação
- ✓ BLE scanner compila sem erro
- ✓ BLE scan executa a cada 15 segundos
- ✓ Serial mostra dispositivos detectados
- ✓ Nenhum crash ou lock-up do sistema

### Notas
- BLE scan é assíncrono
- MAC address de BLE em formato: `XX:XX:XX:XX:XX:XX`
- RSSI típico: -30 (forte) a -90 (fraco)

---

## Task 2: Implementar BLE Service & Caching

**Status:** `[ ] Not Started`  
**Prioridade:** 🔴 CRÍTICA  
**Estimado:** 3-4 horas

### Descrição
Criar serviço BLE com cache similar ao WiFi Service.

### Subtasks
- [ ] Criar `src/models/ble_device.h` com struct BleDevice
- [ ] Criar `src/services/ble_service.h` com classe BleService
- [ ] Implementar `src/services/ble_service.cpp`
- [ ] Singleton pattern: `BleService::getInstance()`
- [ ] Método `scan()` retorna vetor de BleDevice
- [ ] Cache comparado por MAC address (único)
- [ ] Callback para novo device detectado
- [ ] Evitar eventos duplicados
- [ ] Git commit: `feat(service): ble service with caching`

### Critério de Aceitação
- ✓ BleService funciona com cache
- ✓ Novo device gera callback
- ✓ Duplicatas são ignoradas

### Notas
- Usar mesmo padrão que WifiService (Fase 2)
- MAC address é identificador único

---

## Task 3: Implementar GET /api/ble Endpoint

**Status:** `[ ] Not Started`  
**Prioridade:** 🔴 CRÍTICA  
**Estimado:** 2-3 horas

### Descrição
Expor lista de dispositivos BLE via HTTP.

### Subtasks
- [ ] Modificar `src/api_routes.cpp`
- [ ] Adicionar GET `/api/ble` handler
- [ ] Handler chama `BleService::getInstance().scan()`
- [ ] Serializa resultado com JSON helper (ou novo específico para BLE)
- [ ] Retorna: `{success, timestamp, count, data[]}`
- [ ] Testar com curl: `curl http://ESP32_IP/api/ble`
- [ ] Git commit: `feat(api): implement GET /api/ble endpoint`

### Critério de Aceitação
- ✓ GET /api/ble retorna JSON válido
- ✓ JSON contém dispositivos BLE
- ✓ Status 200 OK

---

## Task 4: Emitir BLE_UPDATE Events via WebSocket

**Status:** `[ ] Not Started`  
**Prioridade:** 🔴 CRÍTICA  
**Estimado:** 3-4 horas

### Descrição
Transmitir eventos BLE em real-time via WebSocket.

### Subtasks
- [ ] Modificar `src/websocket.cpp`
- [ ] Adicionar função para emitir BLE_UPDATE
- [ ] Callback de novo device BLE chama broadcast
- [ ] JSON format: `{event: "BLE_UPDATE", payload: {name, rssi, uuid}}`
- [ ] Broadcast para todos clientes WS conectados
- [ ] Log: "[INFO] BLE_UPDATE event sent (Device: XXX, RSSI: -60)"
- [ ] Testar com cliente WebSocket
- [ ] Git commit: `feat(events): broadcast BLE_UPDATE events`

### Critério de Aceitação
- ✓ Novo device BLE gera evento
- ✓ Clientes WebSocket recebem evento JSON
- ✓ Múltiplos clientes recebem simultâneos

---

## Task 5: Sincronização WiFi/BLE & Testing

**Status:** `[ ] Not Started`  
**Prioridade:** 🟡 ALTA  
**Estimado:** 3-4 horas

### Descrição
Evitar contenção entre WiFi e BLE, testar tudo junto.

### Subtasks
- [ ] Configurar WiFi scan em 10s, BLE scan em 15s (offset 5s)
- [ ] Alterar intervals em `config.h`
- [ ] Validar não há travamentos com ambos scans rodando
- [ ] Compilar: `platformio run` (zero warnings)
- [ ] Flash: `platformio run --target upload`
- [ ] Testar GET /api/ble: `curl http://ESP32_IP/api/ble`
- [ ] Testar WebSocket recebe BLE_UPDATE events
- [ ] Validar stats contém bleDevices count correto
- [ ] Verificar memory não cresce indefinidamente
- [ ] Validar todos checkpoints em checklist.md
- [ ] Cleanup código, remover dead code
- [ ] Git commit: `refactor(fase3): cleanup and final tests`

### Critério de Aceitação
- ✓ Compilação: Zero erros, <5 warnings
- ✓ HTTP: GET /api/ble funciona
- ✓ WebSocket: BLE_UPDATE events fluem
- ✓ Stats: bleDevices count correto
- ✓ Checklist: 100% checkpoints marcados

---

## Priorização de Tasks

### 🔴 CRÍTICAS
1. Task 1: BLE Scanner
2. Task 2: BLE Service & Caching
3. Task 3: GET /api/ble
4. Task 4: BLE_UPDATE Events

### 🟡 ALTAS
5. Task 5: Sync & Testing

---

## Progresso Global Fase 3

```
[ ] Task 1: BLE Scanner Básico           0%
[ ] Task 2: BLE Service & Caching        0%
[ ] Task 3: GET /api/ble Endpoint        0%
[ ] Task 4: BLE_UPDATE Events            0%
[ ] Task 5: Sync & Testing               0%

TOTAL PROGRESSO FASE 3: 0/5 = 0%
```

---

## Changelog

| Data | Mudança |
|------|---------|
| 22/05/2026 | Criação inicial de tasks Fase 3 |

