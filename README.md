# FansElectronics_DMD32

[🇮🇩 Bahasa Indonesia](README-id.md)

---

## Description 📖
**FansElectronics_DMD32** is a high-performance, flicker-free LED Dot Matrix library specifically designed for **ESP32** microcontrollers.

This library empowers IoT developers and digital display manufacturers to:
- Drive **HUB08 and HUB12 (P10)** panels with ease
- Implement flicker-free animations using **Double Buffering**
- Control brightness efficiently via **Hardware PWM**
- Create complex graphics with a robust coordinate system

---

## What's New in v1.0.0 🚀

Initial release focused on **multi-panel stability and hardware flexibility**.

### Why choose this library?
Unlike generic libraries, DMD32 focuses on ESP32 high-speed performance:
- **Flexible Pin Mapping:** Configure custom GPIOs via simple struct.
- **Hardware SPI:** Utilizes HSPI peripheral for high-speed data transfer.
- **Memory Optimized:** Native heap allocation for framebuffer management.

---

## Features ✨
- Unique **Double Buffer** system for zero-flicker visuals
- Native support for **HUB08 (P4.75, P7.62)** and **HUB12 (P10)**
- Adjustable **Hardware PWM brightness** (LEDC)
- Built-in font rendering engine
- Support for Daisy-chaining multiple panels
- Production-ready for 24/7 digital displays

---

## Supported Panel Modes 🔒

| Panel Type | Scan Rate | Panel Model | Status |
|---|---|---|---|
| HUB08 | 1/16 Scan | P4.75 & P7.62 Single Color | ✅ Implemented |
| HUB12 | 1/4 Scan |  P10 Single Color | ✅ Implemented |

Perfect for:
- Commercial advertising displays
- Home automation status boards
- Moving message (running text) systems
- High-resolution LED matrix installations
- Prayer Times

---

## How It Works ❓

1️⃣ **Initialize:** Define hardware pins and panel type in the configuration struct.  
2️⃣ **Setup:** Call `dmd.begin()` to allocate memory for the matrix canvas.  
3️⃣ **Draw:** Use standard drawing functions (`drawText`, `drawLine`, `drawRect`) on the framebuffer.  
4️⃣ **Refresh:** Execute `dmd.update()` in the loop for high-speed SPI row scanning.

---
## Default Pins ⚙️
```cpp
// Default Pins ESP32 
DmdPins pinsDefault = {
  .pin_A     = 2,    // Changed to GPIO 2 (Safe for output)
  .pin_B     = 26,   // Keep
  .pin_C     = 27,   // Keep (Set to -1 for HUB12)
  .pin_D     = 14,   // Keep (Set to -1 for HUB12)
  .pin_latch = 5,    // Keep
  .pin_sck   = 18,   // Standard VSPI Clock
  .pin_oe    = 4,    // Changed to GPIO 4 (Freed up GPIO 17)
  .pin_mosi  = 23    // Standard VSPI MOSI (High-speed data)
};
```

## Quick Example 🔑

```cpp
#include <FansElectronics_DMD32.h>
#include <fonts/SystemFont5x7.h>

FansElectronics_DMD32 dmd;

// Configure Pins
DmdPins pinsHUB08 = {48, 47, 21, 14, 13, 12, 11, 10};

void setup() {
    dmd.begin(PANEL_HUB08, 2, 1, pinsHUB08); // 2 Panels horizontal
    dmd.setDoubleBuffer(true);
    dmd.setBrightness(128);
}

void loop() {
    dmd.clear();
    dmd.drawText(2, 0, "FansElectronics");
    dmd.swapBuffers();
    dmd.update();
}

```

---

## Created By 💻
- Writer            : Irfan Indra Kurniawan, ST
- Organization      : Fans Electronics
- Website           : www.fanselectronics.com
- Email             : info@fanselectronics.com

> **Author note:** Feel free to develop and use this library for education, personal, religious, or commercial purposes.

---

## Buy me a Coffee ☕
- info@fanselectronics.com
- www.facebook.com/FansElectronicsCom
- www.instagram.com/fanselectronics
- www.saweria.co/fanselectronics

---

## Thanks To 🤲
- Allah Subhanahu Wa Ta'ala
- Arduino.cc
- GitHub
- Contributor
- Everyone who gives me coffee