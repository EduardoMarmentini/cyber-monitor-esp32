# CHECKLIST — Fase 1: Setup, Configuração & WiFi Scanner

**Data Planejada:** 22/05/2026 - 27/05/2026  
**Status:** ⏳ Aguardando Início

---

## Checklist compilação e build

- [ ] **C1.1:** PlatformIO instalado e funcional (`platformio --version`)
- [ ] **C1.2:** Projeto compila sem erros: `platformio run`
- [ ] **C1.3:** Build produz `.elf` binary sem warnings
- [ ] **C1.4:** `platformio run --target upload` realiza flash com sucesso
- [ ] **C1.5:** `platformio device monitor` conecta ao serial (115200 baud)
- [ ] **C1.6:** No refactor/updates de libraries conseguem recompilar

---

## Checklist hardware & conexão WiFi

- [ ] **H1.1:** ESP32 Dev Kit conectado via USB
- [ ] **H1.2:** Driver USB reconhece porta (COM3 / /dev/ttyUSB0)
- [ ] **H1.3:** SSID e password WiFi inseridos corretamente em `config.h`
- [ ] **H1.4:** ESP32 consegue conectar ao WiFi home (visível no serial)
- [ ] **H1.5:** IP address válido atribuído (ex: 192.168.x.x)
- [ ] **H1.6:** Ping responde do ESP32 IP

---

## Checklist WiFi scanner

- [ ] **W1.1:** Função `scanWifiNetworks()` existe em `wifi_scanner.cpp`
- [ ] **W1.2:** Scan executa sem travamentos (non-blocking)
- [ ] **W1.3:** Scan encontra redes próximas (quantidade > 0)
- [ ] **W1.4:** Cada rede detectada tem: SSID, BSSID, RSSI, channel, encryption
- [ ] **W1.5:** Dados coletados estão corretos (RSSI em dBm, channel 1-13, etc)
- [ ] **W1.6:** Loop executa scan a cada 10 segundos (verificar em logs)
- [ ] **W1.7:** Não há memory leaks (verificar com `ESP.getFreeHeap()`)

---

## Checklist HTTP API

- [ ] **A1.1:** HTTP server inicia na porta 80 sem erro
- [ ] **A1.2:** GET http://ESP32_IP/ retorna "OK" (health check)
- [ ] **A1.3:** GET http://ESP32_IP/api/wifi retorna JSON válido
- [ ] **A1.4:** HTTP response status é 200 OK
- [ ] **A1.5:** Content-Type header é application/json
- [ ] **A1.6:** Response time é <100ms
- [ ] **A1.7:** Rota /api/wifi pode ser chamada múltiplas vezes sem erro

---

## Checklist JSON response

- [ ] **J1.1:** JSON estrutura tem campos: `success`, `timestamp`, `count`, `data`
- [ ] **J1.2:** `success` contém boolean true (neste caso sempre true)
- [ ] **J1.3:** `timestamp` contém Unix time em milissegundos
- [ ] **J1.4:** `count` contém número de redes encontradas
- [ ] **J1.5:** `data` é array com objetos WifiNetwork
- [ ] **J1.6:** Cada objeto em `data[]` tem: `ssid`, `bssid`, `rssi`, `channel`, `encryption`
- [ ] **J1.7:** JSON é valido (passou em `jq . < response.json`)

Exemplo JSON esperado:
```json
{
  "success": true,
  "timestamp": 1740000000123,
  "count": 3,
  "data": [
    {
      "ssid": "NETGEAR",
      "bssid": "00:11:22:33:44:55",
      "rssi": -42,
      "channel": 6,
      "encryption": "WPA2"
    }
  ]
}
```

---

## Checklist logging

- [ ] **L1.1:** Serial logs aparecem no monitor (115200 baud)
- [ ] **L1.2:** Logs têm prefixo [INFO], [WARN], ou [ERROR]
- [ ] **L1.3:** Logs têm timestamp relativo (ex: [00:15:23])
- [ ] **L1.4:** Log ao boot: "[INFO] System starting..."
- [ ] **L1.5:** Log ao conectar WiFi: "[INFO] WiFi connected! IP: x.x.x.x"
- [ ] **L1.6:** Log a cada scan: "[INFO] WiFi scan complete: N networks found"
- [ ] **L1.7:** Log de requisições HTTP: "[INFO] GET /api/wifi (200 OK)"

---

## Checklist código & documentação

- [ ] **C2.1:** Código segue convenções: `snake_case` files, `PascalCase` classes, `camelCase` functions
- [ ] **C2.2:** Nenhum código comentado/morto (remover ou explicar)
- [ ] **C2.3:** Funções complexas têm comentários explicativos
- [ ] **C2.4:** `config.h` bem documentado com explicações de cada #define
- [ ] **C2.5:** README.md existe com instruções: clone, build, flash, run
- [ ] **C2.6:** Git history está limpo: não há commits sem mensagem clara

---

## Checklist estrutura de arquivos

- [ ] **S1.1:** Diretório `/firmware` existe e é raiz do projeto
- [ ] **S1.2:** `platformio.ini` está presente e válido
- [ ] **S1.3:** Diretório `src/` contém: main.cpp, config.h, wifi_scanner.h/cpp, api_routes.h/cpp, logger.h/cpp
- [ ] **S1.4:** Diretório `src/models/` contém: wifi_network.h
- [ ] **S1.5:** Diretório `src/utils/` contém: json_helper.h/cpp
- [ ] **S1.6:** .gitignore previne commit de `.*build/`, `*.o`, `*.elf`

---

## Checklist dependências

- [ ] **D1.1:** `platformio.ini` lista: `me-no-dev/ESP Async WebServer`
- [ ] **D1.2:** `platformio.ini` lista: `bblanchon/ArduinoJson@^6.19.0`
- [ ] **D1.3:** `platformio.ini` lista: `AsyncTCP`
- [ ] **D1.4:** Todas libs instalam corretamente (sem conflitos)
- [ ] **D1.5:** Verifica se há atualizações de lib que quebram código

---

## Checklist testes funcionais

### Teste 1: Boot & WiFi Connection
```
[ ] Serial mostra "[INFO] System starting..."
[ ] Serial mostra "[INFO] Connecting to WiFi: SSID_AQUI..."
[ ] Serial mostra "[INFO] WiFi connected! IP: 192.168.x.x"
[ ] Sem erros ou resets inesperados
```

### Teste 2: WiFi Scan
```
[ ] Serial mostra "[INFO] WiFi scan started"
[ ] Após 10s, serial mostra "[INFO] WiFi scan complete: N networks found"
[ ] Lista contém SSIDs válidos (sua rede e vizinhos)
[ ] Repetições de 10s em 10s sem problemas
```

### Teste 3: HTTP Health Check
```terminal
$ curl http://192.168.x.x/
OK
```
```
[ ] Resposta é exatamente "OK"
[ ] Status code é 200
```

### Teste 4: WiFi API Endpoint
```terminal
$ curl http://192.168.x.x/api/wifi
```
```
[ ] Resposta é JSON válido
[ ] Contém campo "success": true
[ ] Contém campo "timestamp" com número válido
[ ] Contém campo "count" com número >= 0
[ ] Contém campo "data" como array
[ ] Sem erros 500
```

### Teste 5: Repeated Requests
```terminal
$ for i in {1..5}; do curl http://192.168.x.x/api/wifi; sleep 1; done
```
```
[ ] Todas 5 respostas são JSON válido
[ ] Sem crashes ou timeouts
[ ] Timestamps aumentam
```

### Teste 6: Memory Health
```
Monitor serial por 5 minutos
```
```
[ ] Free RAM não diminui constantemente (sem memory leak)
[ ] Sem crashes inesperados
[ ] ESP32 mantém conexão WiFi estável
```

---

## Checklist de segurança & robustez

- [ ] **RB1.1:** WiFi password não está em plain text nos logs
- [ ] **RB1.2:** Servidor HTTP responde a invalid requests sem crash
- [ ] **RB1.3:** Timeout configurado para requests longas
- [ ] **RB1.4:** Sem buffer overflows (validar inputs)
- [ ] **RB1.5:** API não expõe informações sensíveis (ex: MAC da antena)

---

## Sign-off Fase 1

**Todos os checkpoints preenchidos?**

- [ ] **SIM** — Marcar fase como ✓ COMPLETA e desbloquear Fase 2
- [ ] **NÃO** — Documentar bloqueadores em "Problemas Encontrados" abaixo

---

## Problemas Encontrados & Resoluções

### Problema 1
**Descrição:** (escrever aqui se encontrar)  
**Data:** N/A  
**Status:** Resolvido/Pendente  
**Solução:** (descrever)  

(_adicionar mais seções conforme problemas surgem_)

---

## Notas Adicionais

- Se ESP32 não conecta ao WiFi, verificar SSID/password e signal strength no local
- Se compilação falha, executar `platformio update` e tentar novamente
- Se upload falha, tentar: resetar ESP32 manualmente ou mudar baud rate em platformio.ini

---

## Changelog

| Data | Mudança |
|------|---------|
| 22/05/2026 | Criação inicial de checklist Fase 1 |

