# FASE 1 — Setup, Configuração & WiFi Scanner

**Status:** ⏳ Aguardando Início  
**Estimado:** 3-5 dias  
**Data Início:** 22/05/2026  
**Data Prevista de Conclusão:** 27/05/2026  
**Responsável:** Firmware Developer

---

## Objetivo da Fase

Ter um **ESP32 bootável, conectado ao WiFi e capaz de escanear redes próximas**, com uma API HTTP básica funcionando.

Ao final desta fase você terá:
- ✓ Projeto compilável e flashável para ESP32 via PlatformIO
- ✓ ESP32 conectando automaticamente ao WiFi home
- ✓ WiFi scanner detectando redes vizinhas
- ✓ Endpoint HTTP `/api/wifi` retornando JSON
- ✓ Logs seriais mostrando progresso e debug

---

## Escopo Detalhado

### **O que está incluído:**

1. **Inicialização PlatformIO**
   - Criar projeto com Arduino Framework
   - Configurar ESP32dev como target board
   - Adicionar dependências obrigatórias (WiFi.h, ESPAsyncWebServer, ArduinoJson)
   - Validar compilação sem erros

2. **Conexão WiFi**
   - Implementar WiFi connection com SSID/Password em `config.h`
   - Conectar automaticamente na startup
   - Mostregar status de conexão no log serial
   - Tratar falhas de conexão (retry logic)

3. **WiFi Scanner**
   - Implementar scan de redes WiFi próximas
   - Coletar: SSID, BSSID, RSSI, channel, encryption type
   - Armazenar resultados em estrutura de dados
   - Executar scans em interval configurável (default 10s)

4. **API REST Básica**
   - Inicializar servidor HTTP na porta 80
   - Implementar endpoint `GET /api/wifi`
   - Retornar lista de redes em formato JSON padronizado
   - Adicionar timestamp à response

5. **JSON Serialization**
   - Usar ArduinoJson para serialização
   - Padronizar estrutura de respostas (success, timestamp, data)
   - Facilitar extensão futura com novos endpoints

6. **Logging Básico**
   - Output no Serial na velocidade 115200 baud
   - Mensagens com timestamps
   - Prefixos [INFO], [ERROR], [WARN]
   - Log eventos: boot, WiFi connect, scan complete, requests recebidas

---

### **O que está FORA de escopo:**

- ❌ WebSocket (fase 2)
- ❌ BLE (fase 3)
- ❌ Logger class completo (fase 2)
- ❌ Watchdog (fase 4)
- ❌ OTA Updates (futuro)
- ❌ MQTT (futuro)

---

## Dependências

**Bloqueadores:** Nenhum (fase inicial)

**Pré-requisitos Hardware:**
- ESP32 Dev Kit (ou compatível)
- Cabo USB para programação
- Rede WiFi disponível (SSID + password)

**Pré-requisitos Software:**
- VS Code com PlatformIO Extension instalada
- Python 3.x (para PlatformIO)
- Git configurado

---

## Arquivos a Serem Criados/Modificados

### **Novos arquivos:**
```
src/
├── main.cpp                    ← Entry point, setup e loop
├── config.h                    ← Constantes globais (WiFi credentials)
├── wifi_scanner.h
├── wifi_scanner.cpp            ← Implementação do scanner
├── api_routes.h
├── api_routes.cpp              ← Inicialização e rutas HTTP
├── models/
│   └── wifi_network.h          ← Struct WifiNetwork
└── utils/
    ├── json_helper.h
    └── json_helper.cpp         ← Helper para JSON serialization

platformio.ini                   ← Configuração de build
README.md                        ← Documentação inicial
```

### **Modificados:**
- Nenhum (projeto novo)

---

## Timeline Detalhada

| Dia | Task | Checkpoint |
|-----|------|-----------|
| 1-2 | Setup PlatformIO + WiFi conexão | Compilação OK, WiFi conecta |
| 2-3 | WiFi scanner implementação | Scanner detecta redes |
| 3-4 | API HTTP basic + JSON | GET /api/wifi retorna JSON |
| 4-5 | Log serial + cleanup | Logs funcionando, código limpo |

---

## Stack Técnico (Fase 1)

| Componente | Versão/Lib | Propósito |
|-----------|-----------|----------|
| Framework | Arduino | Abstração hardware |
| Servidor HTTP | ESP Async WebServer | Rotas HTTP |
| JSON | ArduinoJson 6.x | Serialization |
| WiFi | WiFi.h (built-in) | Conexão WiFi |
| Serial | HardwareSerial | Debug output |

---

## Critérios de Aceitação

Fase 1 é considerada **completa** quando:
- ✓ Projeto compila sem erros (`platformio run`)
- ✓ ESP32 flashable via `platformio run --target upload`
- ✓ ESP32 conecta ao WiFi automaticamente (visível no serial)
- ✓ Scan WiFi executa a cada 10s (logs no serial)
- ✓ GET http://ESP32_IP/api/wifi retorna JSON válido
- ✓ JSON response tem structure: `{success, timestamp, count, data[]}`
- ✓ Todos checkpoints em `checklist.md` marcados ✓
- ✓ Código comentado e sem warnings

---

## Riscos & Mitigações

| Risco | Probabilidade | Impacto | Mitigação |
|-------|---------------|--------|-----------|
| WiFi credenciais erradas | Alta | Bloqueador | Verificar SSID/password em config.h |
| Serial não funciona | Média | Debug difícil | Usar LED blink alternative |
| Librariy conflicts | Baixa | Bloqueador | Resolver via platformio.ini |
| ESP32 firmware desatualizado | Baixa | Comportamento estranho | Fazer erase + program fresh |

---

## Próximas Passos (Após Conclusão)

1. Validar todos checkpoints em `FASE_1/checklist.md`
2. Fazer commit em git: `feat: fase-1-wifi-scanner-complete`
3. **Desbloquear Fase 2:** WebSocket + API REST completa
4. Ler [FASE 2 FASE.md](../fase-2-api-realtime/FASE.md) para preparar próximas tasks

---

## Notas & Observações

- WiFi scan é assíncrono; não bloqueia servidor HTTP
- RSSI (received signal strength) em dBm (negativo)
- Encryption types esperados: Open, WEP, WPA, WPA2, WPA3
- BLE será separado em fase 3

