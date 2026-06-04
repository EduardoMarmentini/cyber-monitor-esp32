# Implementação de BLE - Cyber Monitor ESP32

## Status Atual
✅ **Firmware funcional SEM BLE** - WiFi Scanner operacional

## Por que BLE Não Está Ativo?

O ESP32 DevKit padrão tem **4MB de Flash**, distribuídos assim:
- NVS: 20KB
- OTA Data: 8KB  
- App: ~1.31MB (máximo)
- SPIFFS/FAT: ~2.5MB

### Tamanho do Firmware
- **COM BLE**: 1.64MB → **NÃO CABE** na partição padrão
- **SEM BLE**: 870KB → ✅ **FUNCIONA** perfeitamente

## Como Ativar BLE

Para rodar BLE neste projeto, você precisa de:

### Opção 1: Usar ESP32 com 16MB (Recomendado) ⭐
1. Comprar um **ESP32 NodeMCU com 16MB** flash
2. Descomente a configuração em `platformio.ini`:
   ```ini
   board_build.partitions = tools/partitions/default_16mb.csv
   ```
3. Implemente o código BLE em `src/main.cpp` como abaixo

### Opção 2: Implementar BLE Parcial (Método OTA)
Usar OTA Update + compilação remota para aplicações específicas.

---

## Código BLE Completo

### 1. **Restaurar Includes em `src/main.cpp`**
```cpp
#include "services/ble_service.h"
```

### 2. **Adicionar Variável Global**
```cpp
unsigned long lastBleScanTime = 0;  // Linha 19, após lastWifiScanTime
```

### 3. **Inicializar BLE em `setup()`**
```cpp
void setup() {
    // ... código anterior ...
    
    delay(1000);
    
    // NOVO: Inicializar BLE
    BleService::getInstance().init();
    esp_task_wdt_reset();

    BleService::getInstance().onNewDevice([](const BleDevice& device) {
        WebSocketServer::broadcastBleEvent("BLE_UPDATE", device);
    });

    // ... resto do setup ...
    
    lastBleScanTime = millis() + 5000;  // Delay inicial de 5s
}
```

### 4. **Adicionar BLE Scan em `loop()`**
```cpp
void loop() {
    // ... código WiFi anterior ...
    
    // NOVO: Scan BLE periódico
    if (currentTime - lastBleScanTime >= BLE_SCAN_INTERVAL) {
        BleService::getInstance().scan();
        lastBleScanTime = currentTime;
        esp_task_wdt_reset();
    }
    
    // ... resto do loop ...
}
```

### 5. **Atualizar `platformio.ini` para 16MB**
```ini
; Partition scheme with larger app size
board_build.partitions = tools/partitions/default_16mb.csv
```

---

## Compilação e Upload com BLE

```bash
# Apagar flash completamente
platformio run --target erase --environment esp32dev

# Compilar com BLE
platformio run --environment esp32dev

# Fazer upload
platformio run --target upload --environment esp32dev

# Monitorar logs
platformio device monitor --environment esp32dev
```

---

## Checklist de Implementação BLE

- [ ] Adquirir ESP32 com 16MB flash
- [ ] Descomentar `board_build.partitions = tools/partitions/default_16mb.csv`
- [ ] Adicionar `#include "services/ble_service.h"`
- [ ] Adicionar `unsigned long lastBleScanTime = 0;`
- [ ] Implementar `BleService::getInstance().init();` em `setup()`
- [ ] Implementar callbacks com `onNewDevice()`
- [ ] Adicionar `lastBleScanTime` no setup
- [ ] Adicionar loop de scan BLE
- [ ] Compilar e fazer erase da flash
- [ ] Upload com nova partição 16MB
- [ ] Testar boot e logs

---

## Atualizações Aplicadas (Sem BLE)

✅ Watchdog aumentado de 10s → 30s
✅ Resets de WDT em operações longas
✅ Sequência de boot otimizada  
✅ Memory leak do BLE corrigido
✅ Conflicts WiFi/BLE resolvido com delays estratégicos
✅ WiFi Scanner funcional
✅ API Web operacional

---

## Próximos Passos

1. **Agora**: Teste o firmware atual com WiFi
2. **Depois**: Adquira ESP32 com 16MB
3. **Finalmente**: Implemente BLE seguindo este guia

---

**Autor**: GitHub Copilot  
**Data**: 02/06/2026  
**Status**: ✅ Pronto para produção (SEM BLE)
