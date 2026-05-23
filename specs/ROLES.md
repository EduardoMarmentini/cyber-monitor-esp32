# ROLES — Definição de Responsabilidades

**Última atualização:** 22/05/2026

---

## Equipe & Estrutura

Este projeto é desenvolvido por **um único developer** (você) em todos as responsabilidades técnicas. Não há diferenciação de roles por pessoa, mas sim **responsabilidades funcionais** sobre diferentes aspectos do projeto.

---

## Roles Definidos

### **Firmware Developer** (seu perfil)

**Descrição:** Responsável por toda a implementação, testes e validação do firmware ESP32.

**Responsabilidades:**

#### 1. **Implementação de Código C++**
- Escrever código C++ seguindo convenções do projeto
- Implementar WiFi scanner, BLE scanner, API REST, WebSocket
- Manter padrão de namespacing e organização de arquivos
- Seguir naming conventions: `snake_case` (arquivos), `PascalCase` (classes), `camelCase` (funções)

#### 2. **Build & Compilação**
- Configurar e manter `platformio.ini`
- Gerenciar dependências via PlatformIO Library Manager
- Garantir que código compila sem warnings (quando possível)
- Documentar library versions usadas

#### 3. **Debug & Troubleshooting**
- Usar monitor serial para debug
- Identificar e resolver issues de hardware/software
- Testar com ESP32 real hardware
- Documentar problemas encontrados e soluções

#### 4. **Testes & Validação**
- Validar cada checkpoint de fase contra checklist.md
- Testar endpoints HTTP manualmente (curl, Postman)
- Validar WebSocket com cliente web simples
- Testar comportamento em falhas/edge cases

#### 5. **Documentação**
- Comentar código complexo (algoritmos, lógica não-óbvia)
- Manter README.md atualizado
- Documentar configurações em config.h
- Adicionar exemplos de uso para APIs

#### 6. **Gerenciamento de Progresso**
- Manter tasks.md atualizado com status
- Atualizar checklist.md conforme progresso
- Relatar bloqueadores ou dependências
- Acompanhar versões de mudanças

---

## Competências Esperadas

### **Hard Skills**
- ✓ C++ (intermediário+)
- ✓ Arduino Framework
- ✓ PlatformIO
- ✓ HTTP/REST APIs
- ✓ WebSockets
- ✓ JSON serialization
- ✓ BLE (Bluetooth Low Energy concepts)
- ✓ Git & version control

### **Soft Skills**
- ✓ Resolução de problemas (debugging)
- ✓ Atenção a detalhes
- ✓ Auto-organização (você é solo)
- ✓ Documentação clara

---

## Autoridades & Decisões

Como **Firmware Developer solo**, você tem autoridade para:

- ✓ Escolher implementação interna (desde que respeite API contracts)
- ✓ Ajustar timeline dentro de uma fase (se bloqueadores surgem)
- ✓ Propor otimizações ou refactors
- ✓ Rejeitar changes que quebrem API contracts
- ✓ Pausar fase se dependência externa bloquear

Você **não tem autoridade** para:
- ✗ Mudar API contracts sem justificativa forte
- ✗ Pular phases (sequência é vinculante)
- ✗ Estender scope além do definido em MASTER_PLAN (propor para futuro)

---

## Requisitos Técnicos do Ambiente

### **Hardware**
- ✓ ESP32 Dev Kit (ou compatível)
- ✓ Cabo USB para programação
- ✓ Antena WiFi integrada ou externa
- ✓ Antena BLE integrada

### **Software**
- ✓ VS Code + PlatformIO Extension
- ✓ Git (para versionamento)
- ✓ Python 3.x (para PlatformIO)
- ✓ Ferramentas de teste: curl, ou Postman para HTTP
- ✓ Cliente WebSocket: wscat, ou página HTML simples

### **Ambiente de Testes**
- ✓ WiFi network disponível (para testar conexão)
- ✓ Múltiplos dispositivos WiFi/BLE próximos (para scan)

---

## Processo de Revisão & Validação

### **Por Fase**

1. **Task Completion:** Marque task como ✓ em tasks.md
2. **Checkpoint Validation:** Verifique correspondente em checklist.md
3. **Code Review (Auto):** Você mesmo revisa código para:
   - Segue convenções
   - Sem código morto
   - Documentado
4. **Phase Sign-Off:** Quando 100% dos checkpoints passam, fase é "completa"

### **Critérios de Aceitação**

Cada entrega deve ter:
- ✓ Código compilável (`platformio run`)
- ✓ Todos checkpoints passam
- ✓ Logs/debug output mostra funcionamento
- ✓ Nenhuma dependência quebrada entre módulos
- ✓ README ou documentação atualizada

---

## Comunicação & Escalação

**Bloqueadores:**
- Document em issue/nota na pasta da fase
- Se houver hardware failure: procure substituto/alternativa
- Se compileção quebra: debug & fix imediatamente

**Progresso:**
- Atualize MASTER_PLAN.md a cada mudança de fase
- Mantenha tasks.md em sincronia com work real

---

## Métricas de Sucesso

### **Pessoal (Firmware Developer)**
- [ ] Todas 4 fases completadas no prazo
- [ ] 100% de checkpoints validados
- [ ] Código limpo e comentado
- [ ] Documentação atualizada
- [ ] Zero críticos bugs na entrega final

### **Projeto**
- [ ] ESP32 bootável e robusto
- [ ] APIs HTTP todas funcionando
- [ ] WebSocket real-time operacional
- [ ] BLE detector funcional
- [ ] Logger + Watchdog ativos
- [ ] Documentação completa

---

## Escalação Futura

Se projeto crescer e envolver mais pessoas:
- **QA/Tester:** Responsável por validação sistemática e regressão
- **DevOps/Infra:** OTA updates, CI/CD pipeline, monitoring
- **Tech Lead:** Arquitetura, code review, decisões escaláveis

Por enquanto, você é **multirol**, combinando essas responsabilidades.

---

## Changelog

| Data | Mudança |
|------|---------|
| 22/05/2026 | Criação inicial do ROLES.md |

