# CHECKLIST — Fase 2: API REST Completa & WebSocket Real-time

**Data Planejada:** ~28/05/2026 - 02/06/2026  
**Status:** 🔄 Bloqueada por Fase 1

---

## Checklist WiFi Service & Caching

- [ ] **W2.1:** WifiService classe existe em `src/services/wifi_service.h`
- [ ] **W2.2:** Cache armazena último estado de redes (por BSSID)
- [ ] **W2.3:** Comparação detecta redes novas (não no cache anterior)
- [ ] **W2.4:** Comparação detecta redes removidas (no cache, não agora)
- [ ] **W2.5:** Callback emitido para cada rede nova detectada
- [ ] **W2.6:** Sem eventos duplicados para mesma rede
- [ ] **W2.7:** RSSI changes é trackeado (opcional para Fase 2)

---

## Checklist GET /api/stats Endpoint

- [ ] **S2.1:** GET /api/stats endpoint existe em `api_routes.cpp`
- [ ] **S2.2:** Response JSON contém: `success`, `data`
- [ ] **S2.3:** `data` contém: `wifiNetworks`, `bleDevices`, `uptime`
- [ ] **S2.4:** `wifiNetworks` é inteiro > 0 (valores reais)
- [ ] **S2.5:** `bleDevices` é inteiro == 0 (Fase 2 não tem BLE ainda)
- [ ] **S2.6:** `uptime` é inteiro crescente (segundos desde boot)
- [ ] **S2.7:** HTTP status 200 OK

Exemplo esperado:
```json
{
  "success": true,
  "data": {
    "wifiNetworks": 5,
    "bleDevices": 0,
    "uptime": 120
  }
}
```

---

## Checklist WebSocket Server

- [ ] **WS2.1:** WebSocket server inicializa sem erro
- [ ] **WS2.2:** Log mostra "[INFO] WebSocket server started on ws://..."
- [ ] **WS2.3:** Clientes conseguem conectar com `wscat -c ws://ESP32_IP:81` (ou /ws)
- [ ] **WS2.4:** Log mostra "[INFO] Client connected (total: X)" quando cliente se conecta
- [ ] **WS2.5:** Log mostra "[INFO] Client disconnected (total: X)" quando cliente sai
- [ ] **WS2.6:** Servidor não trava com 5+ clientes simultâneos
- [ ] **WS2.7:** Muitas conexões/desconexões sem memory leak
- [ ] **WS2.8:** Timeout gracioso para conexões idle

---

## Checklist WIFI_UPDATE Events

- [ ] **E2.1:** Evento em JSON format: `{event: "WIFI_UPDATE", payload: {...}}`
- [ ] **E2.2:** Payload contém: `ssid`, `rssi` no mínimo
- [ ] **E2.3:** Quando nova rede detectada, evento é emitido para todos clientes
- [ ] **E2.4:** Cliente WebSocket recebe evento via `onmessage` handler
- [ ] **E2.5:** Múltiplos clientes recebem mesmo evento simultâneos
- [ ] **E2.6:** Sem duplicação de eventos para mesma rede
- [ ] **E2.7:** Log: "[INFO] WIFI_UPDATE event sent (SSID: XXX, RSSI: -40)"

---

## Checklist Logger (Multiple Levels)

- [ ] **L2.1:** Logger::info() funciona e printa no serial
- [ ] **L2.2:** Logger::warn() funciona com prefix [WARN]
- [ ] **L2.3:** Logger::error() funciona com prefix [ERROR]
- [ ] **L2.4:** Logger::debug() funciona (pode ser desativado)
- [ ] **L2.5:** Formato é: `[HH:MM:SS] [LEVEL] Message`
- [ ] **L2.6:** Timestamps são corretos (relativos desde boot)
- [ ] **L2.7:** Todos módulos usam Logger ao invés de Serial.print direto
- [ ] **L2.8:** Log level configurável (WARN+, INFO+, DEBUG)

---

## Checklist Configuração Dinâmica de WiFi (NVS)

- [ ] **N2.1:** Rota `POST /api/config/wifi` existe e aceita JSON com `ssid` e `password`
- [ ] **N2.2:** Rota aceita password opcional (rede aberta: sem password ou "")
- [ ] **N2.3:** Ao receber POST, salva SSID/senha na NVS (Preferences)
- [ ] **N2.4:** Após salvar, tenta conectar na nova rede
- [ ] **N2.5:** Retorna `{ "success": true, "message": "..." }` em caso de sucesso
- [ ] **N2.6:** Retorna `{ "success": false, "message": "..." }` em caso de falha
- [ ] **N2.7:** Rota `GET /api/config/wifi` retorna `{ "ssid": "nome_da_rede" }` (sem senha)
- [ ] **N2.8:** Após reboot, ESP32 carrega config da NVS (fallback config.h se vazio)
- [ ] **N2.9:** Senha não aparece nos logs em nenhum momento
- [ ] **N2.10:** Log mostra "[INFO] WiFi config loaded from NVS: SSID"

---

## Checklist Integração & API Completa

- [ ] **API2.1:** GET /api/wifi funciona (Fase 1, mantém funcionando)
- [ ] **API2.2:** GET /api/ble funciona (por enquanto retorna array vazio [])
- [ ] **API2.3:** GET /api/stats funciona com dados reais
- [ ] **API2.4:** Todos 3 endpoints têm response JSON válido
- [ ] **API2.5:** Content-Type é application/json
- [ ] **API2.6:** HTTP status codes corretos (200, não 500)

---

## Checklist Testes Funcionais

### Teste 1: Health Check
```terminal
$ curl http://192.168.x.x/
OK
```
```
[ ] Resposta é "OK"
[ ] Status 200
```

### Teste 2: WiFi Endpoint (Fase 1 validação)
```terminal
$ curl http://192.168.x.x/api/wifi | jq .
```
```
[ ] JSON válido
[ ] Contém lista de redes
```

### Teste 3: Stats Endpoint
```terminal
$ curl http://192.168.x.x/api/stats | jq .
```
```
[ ] Response contém: success, data.wifiNetworks, data.uptime
[ ] wifiNetworks > 0
[ ] uptime > 0 e crescente
[ ] bleDevices == 0
```

### Teste 4: WebSocket Connection
```terminal
$ wscat -c ws://192.168.x.x:81
```
```
[ ] Conexão sucede sem erro
[ ] Prompt aguardando mensagens
[ ] Digitar qualquer coisa, nenhum crash
[ ] Escape (Ctrl+C) desconecta graciosamente
```

### Teste 5: Broadcast WIFI_UPDATE Event
```
[ ] Conectar 2 clientes WebSocket simultaneamente
[ ] Ligar/desligar device WiFi perto do ESP32
[ ] Ambos clientes recebem evento {event: "WIFI_UPDATE", ...}
[ ] JSON válido em ambos
```

### Teste 6: WiFi Config via API
```terminal
$ curl -X POST http://192.168.x.x/api/config/wifi \
  -H "Content-Type: application/json" \
  -d '{"ssid": "NovaRede", "password": "senha123"}'
```
```
[ ] Response: {"success":true,"message":"Conectado à NovaRede"}
```

```terminal
$ curl http://192.168.x.x/api/config/wifi
```
```
[ ] Response: {"ssid":"NovaRede"}
```

```terminal
# Teste com rede aberta (sem senha)
$ curl -X POST http://192.168.x.x/api/config/wifi \
  -H "Content-Type: application/json" \
  -d '{"ssid": "RedeAberta"}'
```
```
[ ] Conecta em rede aberta sem senha
[ ] Não crasha
```

```terminal
# Teste de persistência
# 1. Mudar rede via POST
# 2. Resetar ESP32 (reboot)
# 3. Ver GET /api/config/wifi
```
```
[ ] Após reboot, SSID ainda é o configurado via POST
[ ] ESP32 conecta automaticamente na rede salva
```

### Teste 7: Memory & Stability
```
Monitor por 10 minutos com servidor rodando
[ ] Free RAM não diminui significativamente
[ ] Sem crashes ou resets
[ ] Logs continuam fluindo normalmente
```

---

## Checklist Código & Estrutura

- [ ] **C2.1:** Diretório `src/services/` criado com wifi_service.h/cpp
- [ ] **C2.2:** Diretório `src/services/` com stats_service.h/cpp
- [ ] **C2.3:** `src/websocket.h/cpp` em lugar correto
- [ ] **C2.4:** Todas classes seguem PascalCase (WifiService, WebSocketServer)
- [ ] **C2.5:** Funções seguem camelCase (scanWifiNetworks, broadcastEvent)
- [ ] **C2.6:** Nenhum código comentado/morto
- [ ] **C2.7:** Arquivos novos têm comentários explicativos no topo

---

## Checklist Documentation Update

- [ ] **D2.1:** README.md atualizado com instruções WebSocket
- [ ] **D2.2:** README menciona endpoints: /api/wifi, /api/stats, /api/ble (coming)
- [ ] **D2.3:** README menciona WebSocket: /ws ou :81
- [ ] **D2.4:** Exemplo de evento WIFI_UPDATE no README
- [ ] **D2.5:** Build/flash instruções continuam válidas
- [ ] **D2.6:** README menciona POST/GET /api/config/wifi com exemplos

---

## Sign-off Fase 2

**Todos os checkpoints preenchidos?**

- [ ] **SIM** — Marcar fase como ✓ COMPLETA, desbloquear Fase 3
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

- Se WebSocket trava, checar se há conexões abertas infinitas (não closing)
- Se memory leak, procurar em broadcast function (string allocation)
- Se eventos duplicados, validar comparação de cache (BSSID matching)

---

## Changelog

| Data | Mudança |
|------|---------|
| 22/05/2026 | Criação inicial de checklist Fase 2 |

