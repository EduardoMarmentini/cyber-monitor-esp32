# FASE 2 — API REST Completa & WebSocket Real-time

**Status:** 🔄 Bloqueada por Fase 1  
**Estimado:** 4-6 dias  
**Data Início:** ~28/05/2026 (após Fase 1)  
**Data Prevista de Conclusão:** ~02/06/2026  
**Responsável:** Firmware Developer

**Pré-requisito:** ✓ Fase 1 COMPLETA

---

## Objetivo da Fase

Complementar API HTTP com endpoint `/api/stats` e implementar comunicação **real-time via WebSocket** com eventos automáticos quando redes WiFi são detectadas.

Ao final desta fase você terá:
- ✓ Todos 3 endpoints HTTP funcionando (`/api/wifi`, `/api/ble`, `/api/stats`)
- ✓ Servidor WebSocket em `/ws` aceitando conexões
- ✓ Eventos `WIFI_UPDATE` propagando em real-time
- ✓ Cache de redes WiFi evitando duplicatas
- ✓ Logger padronizado com suporte a múltiplos níveis
- ✓ Tratamento robusto de conexões

---

## Escopo Detalhado

### **O que está incluído:**

1. **Endpoint GET /api/stats**
   - Retorna contadores: wifiNetworks, bleDevices, uptime
   - Implementar em `stats_service.cpp`
   - Rastrear uptime com millis()

2. **WebSocket Server**
   - Inicializar servidor WS em `/ws`
   - Aceitar múltiplos clientes
   - Gerenciar conexões (connect/disconnect)

3. **Eventos WIFI_UPDATE**
   - Emitir quando nova rede detectada
   - Payload: {event, payload {ssid, rssi}}
   - Broadcast para todos clientes conectados

4. **Cache de Redes WiFi**
   - Armazenar último estado de redes
   - Comparar novo scan com cache
   - Evitar duplicatas e updates desnecessários

5. **Logger Padronizado**
   - Expandir Logger básico (Fase 1)
   - Suporte a [INFO], [WARN], [ERROR], [DEBUG]
   - Output no Serial com timestamp
   - Fase 4 adicionará SPIFFS logging

6. **Tratamento de Desconexões**
   - WebSocket graceful disconnect
   - Reconnect automático (cliente-side)
   - Não travar servidor em conexões ruins

7. **Configuração Dinâmica de WiFi**
   - Rota `POST /api/config/wifi` para alterar SSID/senha em runtime
   - Persistência via NVS (Preferences.h) — não precisa recompilar
   - Suporte a redes abertas (password opcional)
   - Resposta com status da tentativa de conexão
   - Rota `GET /api/config/wifi` para ver SSID atual (sem expor senha)

---

### **O que está FORA de escopo:**

- ❌ BLE (fase 3)
- ❌ Watchdog (fase 4)
- ❌ MQTT (futuro)
- ❌ Autenticação/CORS (futuro)
- ❌ Múltiplas redes salvas com fallback (apenas última config usada)
- ❌ Interface web embarcada (SPIFFS) para config (futuro)

---

## Dependências

**Bloqueadores:** ✓ Fase 1 completa

**Pré-requisitos:**
- WiFi scanner funcionando (Fase 1)
- API routes configuradas (Fase 1)
- JSON helper implementado (Fase 1)

---

## Arquivos a Serem Criados/Modificados

### **Novos arquivos:**
```
src/
├── websocket.h
├── websocket.cpp                 ← WebSocket server
├── services/
│   ├── wifi_service.h
│   ├── wifi_service.cpp          ← WiFi caching & logic
│   └── stats_service.h/cpp       ← Stats endpoint
└── (expandir logger de Fase 1)
```

### **Modificados:**
- `src/api_routes.cpp` — adicionar GET /api/stats e GET/POST /api/config/wifi
- `src/main.cpp` — ler config WiFi da NVS ao invés de config.h
- `src/logger.h/cpp` — expandir de básico para completo
- `src/main.cpp` — inicializar WebSocket + serviços

---

## Timeline Detalhada

| Dia | Task | Checkpoint |
|-----|------|-----------|
| 1 | Implementar WiFi caching + service | Cache funcionando |
| 2 | Adicionar GET /api/stats | Endpoint funciona |
| 3 | Implementar WebSocket server | Clientes conectam |
| 4 | Emitir WIFI_UPDATE events | Eventos fluem |
| 5 | Expandir Logger | Todos níveis funcionam |
| 6 | WiFi config endpoint + NVS | POST /api/config/wifi funciona |
| 7 | Testing & cleanup | 100% checkpoints |

---

## Critérios de Aceitação

Fase 2 é considerada **completa** quando:
- ✓ GET /api/stats retorna JSON com wifiNetworks, bleDevices, uptime
- ✓ WebSocket conecta em `ws://ESP32_IP/ws`
- ✓ Eventos WIFI_UPDATE emitidos quando rede detectada
- ✓ Cache evita eventos duplicados
- ✓ Logger com suporte a INFO, WARN, ERROR, DEBUG
- ✓ Sem memory leaks mesmo com múltiplos clientes WS
- ✓ POST /api/config/wifi altera SSID/senha e persiste após reboot
- ✓ GET /api/config/wifi retorna SSID atual (sem expor senha)
- ✓ Todos checkpoints em `checklist.md` marcados ✓

---

## Próximas Passos (Após Conclusão)

1. Validar todos checkpoints em `FASE_2/checklist.md`
2. Git commit: `feat: fase-2-api-websocket-complete`
3. **Desbloquear Fase 3:** BLE Scanner
4. Ler [FASE 3 FASE.md](../fase-3-ble/FASE.md)

