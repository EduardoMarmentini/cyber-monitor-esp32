# FASE 3 — BLE Scanner & Integração

**Status:** 🔄 Bloqueada por Fases 1-2  
**Estimado:** 4-6 dias  
**Data Início:** ~03/06/2026 (após Fase 2)  
**Data Prevista de Conclusão:** ~08/06/2026  
**Responsável:** Firmware Developer

**Pré-requisito:** ✓ Fases 1 & 2 COMPLETAS

---

## Objetivo da Fase

Implementar scanner BLE (Bluetooth Low Energy) para detectar dispositivos próximos e transmitir dados em real-time.

Ao final desta fase você terá:
- ✓ BLE scanner detectando dispositivos próximos
- ✓ Endpoint `/api/ble` retornando lista de dispositivos BLE
- ✓ Eventos `BLE_UPDATE` propagando via WebSocket
- ✓ Cache de dispositivos BLE
- ✓ Sincronização entre WiFi e BLE scans (sem contenção)
- ✓ Tratamento de erros BLE

---

## Escopo Detalhado

### **O que está incluído:**

1. **BLE Scanner**
   - Usar biblioteca BLEDevice.h (ESP32 built-in)
   - Escanear dispositivos BLE próximos
   - Coletar: name, MAC address, RSSI, UUID

2. **Modelo de Dados BLE**
   - Struct `BleDevice` em `models/ble_device.h`
   - Armazenar em vector/array local

3. **Cache de Dispositivos BLE**
   - Similar ao WiFi (Fase 2)
   - Evitar eventos duplicados
   - Detectar novo vs. removido

4. **Endpoint GET /api/ble**
   - Retornar lista de dispositivos BLE em JSON
   - Mesmo formato que /api/wifi

5. **Eventos BLE_UPDATE**
   - Emitir quando novo dispositivo BLE detectado
   - Broadcast via WebSocket para todos clientes
   - Payload: {event, payload {name, rssi, uuid}}

6. **BLE Service**
   - Classe `BleService` para lógica BLE
   - Singleton pattern
   - Thread-safe (considerar RTOS)

7. **Sincronização WiFi/BLE**
   - Escanearar WiFi e BLE em horários diferentes
   - Evitar interferência (ambos em 2.4GHz)
   - Usar timing: WiFi 10s, BLE 15s (offset)

---

### **O que está FORA de escopo:**

- ❌ BLE GATT (just GAP scan)
- ❌ BLE connection/pairing (only detection)
- ❌ Watchdog (fase 4)
- ❌ OTA (futuro)

---

## Dependências

**Bloqueadores:** ✓ Fases 1-2 completas

**Pré-requisitos:**
- WiFi scanner + WebSocket funcionando (Fases 1-2)
- API structure em place (Fases 1-2)

---

## Arquivos a Serem Criados/Modificados

### **Novos arquivos:**
```
src/
├── ble_scanner.h
├── ble_scanner.cpp
├── models/
│   └── ble_device.h
└── services/
    ├── ble_service.h
    └── ble_service.cpp
```

### **Modificados:**
- `src/api_routes.cpp` — adicionar GET /api/ble
- `src/websocket.cpp` — adicionar BLE_UPDATE events
- `src/main.cpp` — inicializar BLE scanner
- `src/services/stats_service.cpp` — atualizar bleDevices count

---

## Timeline Detalhada

| Dia | Task | Checkpoint |
|-----|------|-----------|
| 1 | Implementar BLE scanner básico | Scanner detecta dispositivos |
| 2 | Implementar BLE service + caching | Cache funcionando |
| 3 | Adicionar GET /api/ble | Endpoint funciona |
| 4 | Emitir BLE_UPDATE events | Eventos fluem no WebSocket |
| 5 | Sincronização WiFi/BLE | Sem contenção |
| 6 | Testing & cleanup | 100% checkpoints |

---

## Critérios de Aceitação

Fase 3 é considerada **completa** quando:
- ✓ GET /api/ble retorna JSON com lista de dispositivos BLE
- ✓ BLE_UPDATE eventos emitidos quando novo device detectado
- ✓ Cache evita duplicatas
- ✓ WiFi e BLE scans não interferem um com outro
- ✓ All stats incluem bleDevices count correto
- ✓ Todos checkpoints em `checklist.md` marcados ✓

---

## Próximas Passos (Após Conclusão)

1. Validar todos checkpoints em `FASE_3/checklist.md`
2. Git commit: `feat: fase-3-ble-complete`
3. **Desbloquear Fase 4:** Robustez & Finalização
4. Ler [FASE 4 FASE.md](../fase-4-ops-finais/FASE.md)

