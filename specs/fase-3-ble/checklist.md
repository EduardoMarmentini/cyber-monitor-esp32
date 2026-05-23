# CHECKLIST — Fase 3: BLE Scanner & Integração

**Data Planejada:** ~03/06/2026 - 08/06/2026  
**Status:** 🔄 Bloqueada por Fases 1-2

---

## Checklist BLE Scanner

- [ ] **B3.1:** Função `scanBleDevices()` existe em `ble_scanner.cpp`
- [ ] **B3.2:** BLE scanner inicializa sem crash
- [ ] **B3.3:** Scan detecta dispositivos BLE próximos
- [ ] **B3.4:** Cada device tem: name, MAC, RSSI, UUID
- [ ] **B3.5:** Scan executa a cada 15 segundos (ou configurado)
- [ ] **B3.6:** Dados coletados são válidos (MAC em XX:XX format, RSSI em dBm)
- [ ] **B3.7:** Sem memory leaks (libre RAM stável)
- [ ] **B3.8:** Sem crashedores ou resets inesperados

---

## Checklist BLE Service & Caching

- [ ] **S3.1:** BleService classe existe em `src/services/ble_service.h`
- [ ] **S3.2:** Singleton pattern implementado corretamente
- [ ] **S3.3:** Cache armazena último estado de devices
- [ ] **S3.4:** Comparação detecta dispositivos novos (não no cache anterior)
- [ ] **S3.5:** Comparação detecta dispositivos removidos
- [ ] **S3.6:** Callback emitido para cada device novo
- [ ] **S3.7:** Sem eventos duplicados para mesmo dispositivo
- [ ] **S3.8:** Stats_service atualiza bleDevices count de BleService

---

## Checklist GET /api/ble Endpoint

- [ ] **API3.1:** GET /api/ble endpoint existe em `api_routes.cpp`
- [ ] **API3.2:** Response JSON contém: `success`, `timestamp`, `count`, `data`
- [ ] **API3.3:** `data` é array de dispositivos BLE
- [ ] **API3.4:** Cada dispositivo tem: `name`, `mac`, `rssi`, `uuid`
- [ ] **API3.5:** HTTP status 200 OK
- [ ] **API3.6:** Content-Type é application/json
- [ ] **API3.7:** Response válido em jq: `curl .../api/ble | jq .`

Exemplo esperado:
```json
{
  "success": true,
  "timestamp": 1740000000123,
  "count": 2,
  "data": [
    {
      "name": "Galaxy Buds",
      "mac": "11:22:33:44:55:66",
      "rssi": -61,
      "uuid": "180D"
    }
  ]
}
```

---

## Checklist BLE_UPDATE Events

- [ ] **E3.1:** Evento em JSON format: `{event: "BLE_UPDATE", payload: {...}}`
- [ ] **E3.2:** Payload contém: `name`, `rssi`, `uuid` no mínimo
- [ ] **E3.3:** Quando novo device BLE detectado, evento é emitido
- [ ] **E3.4:** Todos clientes WebSocket recebem evento simultâneos
- [ ] **E3.5:** Sem duplicação de eventos para mesmo device
- [ ] **E3.6:** Log: "[INFO] BLE_UPDATE event sent (Device: XXX, RSSI: -xx)"
- [ ] **E3.7:** Múltiplas conexões WS recebem mesmo evento

---

## Checklist Sincronização WiFi/BLE

- [ ] **SYNC3.1:** WiFi scan ocorre a cada 10 segundos
- [ ] **SYNC3.2:** BLE scan ocorre a cada 15 segundos
- [ ] **SYNC3.3:** Scans não ocorrem simultaneamente (offset 5s)
- [ ] **SYNC3.4:** Ambos scans funcionam sem travamentos
- [ ] **SYNC3.5:** Sem reset inesperado (watchdog)
- [ ] **SYNC3.6:** Sem memory leak mesmo com ambos rodando
- [ ] **SYNC3.7:** Logs mostram ambos scans executando periodicamente

---

## Checklist API Completa

- [ ] **FULL3.1:** GET / funciona (health)
- [ ] **FULL3.2:** GET /api/wifi funciona (WiFi networks)
- [ ] **FULL3.3:** GET /api/ble funciona (BLE devices)
- [ ] **FULL3.4:** GET /api/stats funciona (stats com bleDevices)
- [ ] **FULL3.5:** Todos 4 endpoints retorna JSON válido
- [ ] **FULL3.6:** WebSocket emite WIFI_UPDATE e BLE_UPDATE

---

## Checklist Testes Funcionais

### Teste 1: BLE Scanner Detection
```
Monitor serial por 30 segundos
[ ] BLE scan inicia
[ ] Log mostra "[INFO] BLE scan complete: N devices found"
[ ] Lista de dispositivos BLE aparece
```

### Teste 2: GET /api/ble Endpoint
```terminal
$ curl http://192.168.x.x/api/ble | jq .
```
```
[ ] Response é JSON válido
[ ] Contém lista de devices BLE
[ ] Cada device tem name, mac, rssi, uuid
[ ] Status 200
```

### Teste 3: Statistics Update
```terminal
$ curl http://192.168.x.x/api/stats | jq .data.bleDevices
```
```
[ ] bleDevices > 0 (ou N devices found)
[ ] Número match com GET /api/ble count
```

### Teste 4: WebSocket BLE Events
```
[ ] Cliente WS conectado
[ ] Novo dispositivo BLE próximo
[ ] Cliente recebe BLE_UPDATE event
[ ] JSON válido com name, rssi, uuid
```

### Teste 5: Dual Scanning (WiFi + BLE)
```
Monitor por 5 minutos
[ ] WiFi scan a cada 10s (log)
[ ] BLE scan a cada 15s (log com offset)
[ ] Sem travamentos  ou crashes
[ ] Free memory não diminui significativamente
```

### Teste 6: Multiple Clients
```
[ ] 2+ clientes WebSocket conectados
[ ] Novo device BLE detectado
[ ] Ambos clientes recebem BLE_UPDATE event
[ ] Sem delays ou perdas
```

---

## Checklist Código & Estrutura

- [ ] **C3.1:** Diretório `src/models/` contém `ble_device.h`
- [ ] **C3.2:** Diretório `src/services/` contém `ble_service.h/cpp`
- [ ] **C3.3:** Arquivo `ble_scanner.h/cpp` existe
- [ ] **C3.4:** Nenhum código comentado/morto
- [ ] **C3.5:** Funções bem documentadas
- [ ] **C3.6:** Convenções mantidas (snake_case, PascalCase, camelCase)

---

## Checklist Documentation

- [ ] **D3.1:** README.md atualizado com info BLE
- [ ] **D3.2:** README menciona endpoint /api/ble
- [ ] **D3.3:** README menciona BLE_UPDATE event
- [ ] **D3.4:** Exemplo de evento BLE_UPDATE no README
- [ ] **D3.5:** Build/flash instruções continuam válidas

---

## Sign-off Fase 3

**Todos os checkpoints preenchidos?**

- [ ] **SIM** — Marcar fase como ✓ COMPLETA, desbloquear Fase 4
- [ ] **NÃO** — Documentar bloqueadores em "Problemas Encontrados" abaixo

---

## Problemas Encontrados & Resoluções

### Problema 1
**Descrição:** (documentar conforme problemas surgem)  
**Data:** N/A  
**Status:** Resolvido/Pendente  
**Solução:** ...

(_adicionar mais seções_)

---

## Notas Adicionais

- Se BLE scan trava, checar se está fazendo reset do ESP32
- Se eventos duplicados, validar MAC address matching em cache
- Se memory leak, procurar em alocação do scan vector

---

## Changelog

| Data | Mudança |
|------|---------|
| 22/05/2026 | Criação inicial de checklist Fase 3 |

