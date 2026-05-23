# MASTER PLAN — Firmware ESP32 Cyber Monitor

**Última atualização:** 22/05/2026  
**Status:** Em planejamento e início de implementação  
**Responsável:** Firmware Developer (solo)

---

## Visão Geral

Plano de execução em **4 fases sequenciais** para implementação do firmware ESP32 conforme especificado no SDD. Cada fase possui objetivos claros, dependências explícitas e checkpoints de validação.

| Fase | Título | Duração | Status |
|------|--------|---------|--------|
| 1 | Setup, Config & WiFi Scanner | 3-5 dias | ⏳ Próxima |
| 2 | API REST Completa & WebSocket | 4-6 dias | 🔄 Bloqueada por Fase 1 |
| 3 | BLE Scanner & Integração | 4-6 dias | 🔄 Bloqueada por Fases 1-2 |
| 4 | Robustez, Logs & Finalização | 3-4 dias | 🔄 Bloqueada por Fases 1-3 |

**Duração Total Estimada:** 14-21 dias  
**Data Estimada de Conclusão:** ~10-17 de Junho de 2026

---

## Estrutura de Fases

### **FASE 1: Setup, Configuração & WiFi Scanner**

**Objetivo Principal:** Ter um ESP32 bootável, conectado ao WiFi e capaz de escanear redes, com API HTTP operacional.

**Escopo:**
- Inicializar projeto PlatformIO com Arduino Framework
- Configurar conexão WiFi com SSID/senha
- Implementar WiFi scanner (scan de redes próximas)
- Criar API REST com endpoint `GET /api/wifi`
- Serializar resposta em JSON padronizado
- Logs básicos no monitor serial

**Dependências:** Nenhuma (início do projeto)

**Entrega Expected:**
- ✓ Projeto compilável e flashável para ESP32
- ✓ Conexão automática ao WiFi
- ✓ Scan completo de redes vizinhas
- ✓ Endpoint /api/wifi retornando JSON com lista de redes
- ✓ Logs seriais mostrando progresso

**Próxima Fase Desbloqueada:** Sim, com sucesso em todos checkpoints

---

### **FASE 2: API REST Completa & WebSocket Real-time**

**Objetivo Principal:** Complementar API HTTP com stats e implementar comunicação real-time via WebSocket com eventos.

**Escopo:**
- Implementar endpoint `GET /api/stats` (contadores e uptime)
- Configurar servidor WebSocket em `/ws`
- Emitir eventos `WIFI_UPDATE` quando redes são encontradas
- Implementar cache de redes WiFi para evitar duplicatas
- Sistema de logs padronizado (Logger class)
- Gerenciar reconexões e desconexões
- Configuração dinâmica de WiFi via API (`POST/GET /api/config/wifi`)
- Persistência em NVS (Preferences.h) — sem necessidade de recompilar

**Dependências:** Fase 1 ✓ completa

**Entrega Expected:**
- ✓ Todos 3 endpoints HTTP funcionando corretamente
- ✓ Clientes podem conectar ao WebSocket
- ✓ Eventos WIFI_UPDATE fluem em real-time
- ✓ Cache evita envio de duplicatas
- ✓ Tratamento de desconexões gracioso

**Próxima Fase Desbloqueada:** Sim, com sucesso em todos checkpoints

---

### **FASE 3: BLE Scanner & Integração**

**Objetivo Principal:** Integrar scanner BLE para detectar dispositivos Bluetooth próximos.

**Escopo:**
- Implementar BLE scanner (biblioteca BLEDevice ESP32)
- Detectar dispositivos BLE e armazenar em cache
- Criar endpoint `GET /api/ble` retornando lista de dispositivos
- Emitir eventos `BLE_UPDATE` via WebSocket
- Sincronizar scans WiFi e BLE (evitar contenção)
- Melhorar tratamento de erros BLE

**Dependências:** Fases 1 e 2 ✓ completas

**Entrega Expected:**
- ✓ BLE scanner detecta e lista dispositivos próximos
- ✓ Endpoint /api/ble retorna JSON formatado
- ✓ Eventos BLE_UPDATE fluem via WebSocket
- ✓ Cache de dispositivos BLE funcional
- ✓ Sem travamentos ou conflitos com WiFi scan

**Próxima Fase Desbloqueada:** Sim, com sucesso em todos checkpoints

---

### **FASE 4: Robustez, Otimizações & Finalização**

**Objetivo Principal:** Hardening do firmware, otimizações de performance e documentação completa.

**Escopo:**
- Implementar Logger completo com múltiplos níveis (INFO, WARN, ERROR)
- Configurar watchdog para auto-reset em travamentos
- Melhorar tratamento de erros em todos módulos
- Otimizar uso de memória RAM
- Adicionar timeouts para scans longos
- Documentação completa (README, comentários de código)
- Limpeza e refactor do código

**Dependências:** Fases 1, 2 e 3 ✓ completas

**Entrega Expected:**
- ✓ Logger padronizado em todos módulos
- ✓ Watchdog monitorando e resetando
- ✓ Erros tratados graciosamente
- ✓ Performance otimizada
- ✓ Código comentado e limpo
- ✓ README.md completo com instruções de build/flash

**Próxima Fase Desbloqueada:** N/A (última fase) — Projeto finalizado

---

## Dependências Entre Fases

```
┌─────────────┐
│  FASE 1     │ ← WiFi Scanner + API básica
│ (Setup)     │
└──────┬──────┘
       │
       ▼
┌─────────────┐
│  FASE 2     │ ← REST + WebSocket
│ (API REST)  │
└──────┬──────┘
       │
       ├─────────────┐
       │             │
       ▼             │
┌─────────────┐      │
│  FASE 3     │ ←────┘ ← BLE
│ (BLE)       │
└──────┬──────┘
       │
       ▼
┌─────────────┐
│  FASE 4     │ ← Robustez
│ (Finaliz.)  │
└─────────────┘
```

**Nota:** Cada fase bloqueia as subsequentes até que TODOS os checkpoints sejam validados.

---

## Arquivos a Serem Criados (Checklist)

### Estrutura de Diretórios Esperada

```
/firmware
├── include/
├── lib/
├── src/
│   ├── main.cpp
│   ├── config.h
│   ├── wifi_scanner.cpp
│   ├── wifi_scanner.h
│   ├── ble_scanner.cpp
│   ├── ble_scanner.h
│   ├── api_routes.cpp
│   ├── api_routes.h
│   ├── websocket.cpp
│   ├── websocket.h
│   ├── logger.cpp
│   ├── logger.h
│   ├── models/
│   │   ├── wifi_network.h
│   │   ├── ble_device.h
│   │   └── stats.h
│   ├── services/
│   │   ├── wifi_service.cpp
│   │   ├── wifi_service.h
│   │   ├── ble_service.cpp
│   │   ├── ble_service.h
│   │   ├── stats_service.cpp
│   │   └── stats_service.h
│   └── utils/
│       ├── json_helper.cpp
│       └── json_helper.h
├── platformio.ini
└── README.md
```

---

## Roles & Responsabilidades

Veja [ROLES.md](ROLES.md) para detalhes completos.

**Resumido:**
- **Firmware Developer** (você): Implementação de todo código C++, compilação, debug, testes.

---

## Como Acompanhar Progresso

1. **Por fase:** Acesse a pasta `fase-X-*` correspondente
2. **tasks.md:** Lista completa de tasks com subtasks e status
3. **checklist.md:** Validação dos requisitos da fase
4. **FASE.md:** Objetivos, dependências, escopo detalhado

Cada subpasta com `[ ]` unchecked deve ser completada antes de marcar a fase como "Pronta".

---

## Decisões Arquiteturais

- **Sequência:** Fases executadas **uma após a outra** (sem paralelização)
- **Solo development:** Um único developer em todas responsabilidades
- **Stack fixo:** Arduino Framework + PlatformIO (conforme SDD)
- **Escopo:** Roadmap de 4 sprints mapeado para 4 fases
- **Nível de detalhe:** Alto nível com dependências explícitas (não granular)
- **Futures out of scope:** OTA updates, MQTT, OLED, GPS, SD card (futuras expansões)

---

## Contato & Perguntas

- Dúvidas técnicas? Consulte a pasta da fase em questão
- Quer priorizar algo? Verifique `config.md` da fase
- Encontrou um bloqueador? Documente em `checklist.md`

---

## Histórico de Mudanças

| Data | Evento |
|------|--------|
| 22/05/2026 | Criação inicial do MASTER_PLAN |

