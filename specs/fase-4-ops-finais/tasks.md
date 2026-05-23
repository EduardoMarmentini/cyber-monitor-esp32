# TASKS — Fase 4: Robustez, Otimizações & Finalização

**Status:** ⏳ Bloqueada por Fases 1-3  
**Total Tasks:** 5 principais

---

## Task 1: Implementar Logger Completo

**Status:** `[ ] Not Started`  
**Prioridade:** 🔴 CRÍTICA  
**Estimado:** 3-4 horas

### Descrição
Expandir Logger básico (Fase 2) para suporte completo com níveis e configuração.

### Subtasks
- [ ] Expandir `src/logger.h` com classe Logger
- [ ] Implementar enum LogLevel: DEBUG, INFO, WARN, ERROR, CRITICAL
- [ ] Função `Logger::setLevel(LogLevel)` para controlar verbosidade
- [ ] Implementar timestamp relativo em formato [MM:SS.ms]
- [ ] Adicionar module/function name em logs (opcional)
- [ ] Usar printf-style strings: `Logger::info("WiFi: %s", ssid.c_str())`
- [ ] Buffer circular para últimos 1000 logs (opcional)
- [ ] Implementar `src/logger.cpp`
- [ ] Log level pode ser ajustado dinamicamente
- [ ] Git commit: `feat(logger): complete logging system`

### Critério de Aceitação
- ✓ Logger compila sem erro
- ✓ Todos níveis (DEBUG, INFO, WARN, ERROR, CRITICAL) funcionam
- ✓ Logs aparecem no serial com timestamp
- ✓ Nenhum memory leak (buffer circular stable)
- ✓ Verbosidade pode ser reduzida (ex: apenas WARN+)

### Notas
- Fase 4 pode adicionar SPIFFS logging (nice-to-have)
- Usar F() macros para salvar memory (strings em PROGMEM)

---

## Task 2: Implementar Watchdog Timer

**Status:** `[ ] Not Started`  
**Prioridade:** 🔴 CRÍTICA  
**Estimado:** 2-3 horas

### Descrição
Configurar watchdog para auto-reset em caso de travamento.

### Subtasks
- [ ] Configurar hardware watchdog do ESP32
- [ ] Timeout padrão: 10 segundos
- [ ] Feed watchdog em main loop: `feedWatchdog()`
- [ ] Setup em `src/main.cpp`
- [ ] Log de restart: "[CRITICAL] Watchdog reset triggered!"
- [ ] Comportamento esperado: se loop > 10s, reset automático
- [ ] Testar desligando/pausando loop (não chamar feedWatchdog)
- [ ] Log mostra reset reason ao boot
- [ ] Git commit: `feat(watchdog): hardware watchdog protection`

### Critério de Aceitação
- ✓ Watchdog inicializa sem erro
- ✓ Loop normal não é afetado
- ✓ Se loop trava, ESP32 reseta sozinho (em ~10s)
- ✓ Log registra restart

### Notas
- Usar `esp_task_wdt_init()` e `esp_task_wdt_feed()`
- Feed deve ser chamado a cada cycle (< 10s)
- Timeout configurável em config.h

---

## Task 3: Tratamento de Erros & Graceful Degradation

**Status:** `[ ] Not Started`  
**Prioridade:** 🟡 ALTA  
**Estimado:** 4-5 horas

### Descrição
Adicionar tratamento robusto em todos módulos.

### Subtasks
- [ ] Revisar `wifi_scanner.cpp` — adicionar try-catch/checks
- [ ] Revisar `ble_scanner.cpp` — adicionar try-catch/checks
- [ ] Revisar `api_routes.cpp` — validar inputs, timeout
- [ ] Revisar `websocket.cpp` — handle disconnects gracefully
- [ ] Adicionar checks para NULL pointers
- [ ] Validar malloc returns
- [ ] Timeout para operações I/O
- [ ] Fallback behavior se scan falha (retry, log, continue)
- [ ] Nenhum Exception não-catcheada
- [ ] Git commit: `refactor(error-handling): robust error handling`

### Critério de Aceitação
- ✓ Nenhum crash em operações normais
- ✓ Graceful degradation em falhas (log + continue)
- ✓ Memory é liberado em erro
- ✓ Watchdog não disparado por erro (sistema recupera)

### Notas
- Evitar try-catch em main loop (performance)
- Usar return codes para erros
- Log WARN antes de fallback

---

## Task 4: Memory Optimization & Benchmarking

**Status:** `[ ] Not Started`  
**Prioridade:** 🟡 ALTA  
**Estimado:** 3-4 horas

### Descrição
Otimizar uso de memória e performance.

### Subtasks
- [ ] Usar F() macros para strings em PROGMEM
- [ ] Converter Serial.print strings para F("...")
- [ ] Revisar tamanho de buffers (não oversized)
- [ ] Usar `const` para variáveis imutáveis
- [ ] Profile memory por módulo com `ESP.getFreeHeap()`
- [ ] Benchmark latência de endpoints (<100ms target)
- [ ] Testar stability por 30 minutos (memory estável)
- [ ] Documentar memory usage por componente
- [ ] Opcional: desabilitar DEBUG logs se memory aperto
- [ ] Git commit: `refactor(performance): memory and latency optimization`

### Critério de Aceitação
- ✓ Free RAM > 50KB mantido estável
- ✓ API response time < 100ms
- ✓ Nenhum memory leak detectado (30 min test)
- ✓ PROGMEM usage anotado em README

### Notas
- Usar tools PlatformIO para size analysis
- Medir com `size .pio/build/esp32dev/firmware.elf`

---

## Task 5: Documentação Completa & Finalização

**Status:** `[ ] Not Started`  
**Prioridade:** 🟡 ALTA  
**Estimado:** 3-4 horas

### Descrição
Documentação final, cleanup e entrega.

### Subtasks
- [ ] Escrever/atualizar README.md completo
- [ ] README seções: Overview, Features, Hardware, Build, Flash, Usage
- [ ] API documentation com examples (curl, wscat)
- [ ] Troubleshooting guide com problemas comuns
- [ ] Architecture diagram (opcional, ASCII art)
- [ ] Adicionar comentários em código complexo (algorithms, etc)
- [ ] Remover TODO comments (ou move para issues)
- [ ] Revisar todos headers (copyright, description)
- [ ] .gitignore validado (sem commits de build)
- [ ] Compilar final: `platformio run` zero warnings
- [ ] Git commit: `docs(final): complete documentation and cleanup`
- [ ] Final review todos checkpoints
- [ ] Validação 100% das requirements Fase 4
- [ ] Git tag: `v1.0-release` (opcional)

### Critério de Aceitação
- ✓ README.md completo e legível
- ✓ Código comentado e formatado
- ✓ Compilação final zero erros/warnings
- ✓ .gitignore correto
- ✓ Todos checkpoints Fase 4 marcados

### Notas
- README deve ser acionável (alguém consegue buildar do scratch)
- Documentar gotchas e workarounds encontrados
- Adicionar exemplos de uso real

---

## Priorização de Tasks

### 🔴 CRÍTICAS
1. Task 1: Logger Completo
2. Task 2: Watchdog Timer
3. Task 3: Tratamento de Erros

### 🟡 ALTAS
4. Task 4: Memory Optimization
5. Task 5: Documentação & Finalização

---

## Progresso Global Fase 4

```
[ ] Task 1: Logger Completo                 0%
[ ] Task 2: Watchdog Timer                  0%
[ ] Task 3: Error Handling                  0%
[ ] Task 4: Memory Optimization             0%
[ ] Task 5: Documentation & Finalization    0%

TOTAL PROGRESSO FASE 4: 0/5 = 0%
```

---

## Changelog

| Data | Mudança |
|------|---------|
| 22/05/2026 | Criação inicial de tasks Fase 4 |

