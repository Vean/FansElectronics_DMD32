# FansElectronics_DMD32

[🇺🇸 English](README.md)

---

## Deskripsi 📖
**FansElectronics_DMD32** adalah library LED Dot Matrix performa tinggi yang dirancang khusus untuk mikrokontroler **ESP32**.

Library ini membantu pengembang IoT dan produsen perangkat layar digital untuk:
- Mengendalikan panel **HUB08 dan HUB12 (P10)** dengan mudah
- Mengimplementasikan animasi bebas kedip menggunakan **Double Buffering**
- Mengatur tingkat kecerahan secara efisien via **Hardware PWM**
- Membuat grafis kompleks dengan sistem koordinat yang kokoh

---

## Apa yang Baru di v1.0.0 🚀

Rilis perdana yang fokus pada **stabilitas multi-panel dan fleksibilitas perangkat keras**.

### Mengapa memilih library ini?
Berbeda dengan library umum lainnya, DMD32 fokus pada performa tinggi ESP32:
- **Pemetaan Pin Fleksibel:** Konfigurasi GPIO kustom melalui struktur sederhana.
- **Hardware SPI:** Memanfaatkan peripheral HSPI untuk transfer data berkecepatan tinggi.
- **Optimasi Memori:** Alokasi heap bawaan untuk manajemen framebuffer.

---

## Fitur ✨
- Sistem **Double Buffer** yang unik untuk visual tanpa kedip (flicker-free)
- Dukungan bawaan untuk **HUB08 (P4.75, P7.62)** dan **HUB12 (P10)**
- Pengaturan **kecerahan Hardware PWM** (LEDC) yang presisi
- Mesin rendering font bawaan
- Mendukung pemasangan banyak panel secara berantai (*Daisy-chaining*)
- Siap digunakan untuk tampilan digital yang menyala 24/7

---

## Mode Panel yang Didukung 🔒

| Tipe Panel | Kecepatan Scan | Model Panel | Status |
|---|---|---|---|
| HUB08 | 1/16 Scan | P4.75 & P7.62 Single Color | ✅ Implementasi |
| HUB12 | 1/4 Scan | P10 Single Color | ✅ Implementasi |

Sangat cocok untuk:
- Layar iklan komersial
- Papan informasi rumah pintar (Smart Home)
- Sistem teks berjalan (Running Text)
- Instalasi LED matriks resolusi tinggi
- Jadwal Waktu Sholat

---

## Cara Kerja ❓

1️⃣ **Inisialisasi:** Tentukan pin perangkat keras dan tipe panel dalam struktur konfigurasi.  
2️⃣ **Setup:** Panggil `dmd.begin()` untuk mengalokasikan memori bagi kanvas matriks.  
3️⃣ **Menggambar:** Gunakan fungsi menggambar standar (`drawText`, `drawLine`, `drawRect`) pada framebuffer.  
4️⃣ **Refresh:** Jalankan `dmd.update()` di dalam loop untuk pemindaian baris SPI berkecepatan tinggi.

---
## Pin Default ⚙️
```cpp
// Pin Default ESP32 
DmdPins pinsDefault = {
  .pin_A     = 2,    // Diubah ke GPIO 2 (Aman untuk output)
  .pin_B     = 26,   // Tetap
  .pin_C     = 27,   // Tetap (Set ke -1 untuk HUB12)
  .pin_D     = 14,   // Tetap (Set ke -1 untuk HUB12)
  .pin_latch = 5,    // Tetap
  .pin_sck   = 18,   // Standar VSPI Clock
  .pin_oe    = 4,    // Diubah ke GPIO 4 (Membebaskan GPIO 17)
  .pin_mosi  = 23    // Standar VSPI MOSI (Data berkecepatan tinggi)
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

## Dibuat Oleh 💻
- Penulis           : Irfan Indra Kurniawan, ST
- Organisasi        : Fans Electronics
- Website           : www.fanselectronics.com
- Email             : info@fanselectronics.com

> **Catatan Penulis:** Silakan kembangkan dan gunakan library ini untuk tujuan pendidikan, pribadi, keagamaan, maupun komersial.

---

## Buy me a Coffee ☕
- info@fanselectronics.com
- www.facebook.com/FansElectronicsCom
- www.instagram.com/fanselectronics
- www.saweria.co/fanselectronics

---

## Terima Kasih Kepada 🤲
- Allah Subhanahu Wa Ta'ala
- Arduino.cc
- GitHub
- Kontributor
- Semua orang yang memberikan saya kopi