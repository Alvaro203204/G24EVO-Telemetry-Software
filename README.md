# 📡 Requisitos para ejecutar Software de Telemetria G24EVO con WebSocket

A continuación se indican los pasos para configurar **Arduino IDE** y compilar el código.

---

## ✅ Instalación de Placa ESP32 en Arduino IDE

1. Abre **Arduino IDE**.
2. Ve a **Archivo > Preferencias**.
3. En el campo **"Gestor de URLs adicionales de tarjetas"**, añade esta URL: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
4. Luego ve a **Herramientas > Placa > Gestor de tarjetas**.
5. Busca **ESP32** e instala **esp32 by Espressif Systems**.

---

## 📦 Librerías necesarias

Instala las siguientes librerías desde el **Library Manager** o manualmente desde GitHub si no aparecen:

### Desde Library Manager:
- **ESPAsyncWebServer** (by ESP32Async)
- **AsyncTCP** (by dvarrel)

---