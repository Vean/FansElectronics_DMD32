/*
 * Creator             : Irfan Indra Kurniawan, ST
 * Website             : https://www.fanselectronics.com
 * Buy Me a Coffee     : https://saweria.co/FansElectronics
 * Tested on ESP32 Devkit & ESP32-S3 with HUB08 and HUB12 Panels
 * ESP Core 2.0.17 & 3.3.12
 */

// ID: Tambahkan Library
// EN: Add Library
#include <FansElectronics_DMD32.h>

// ID: Tambahkan Font Anda
// EN: Add Your Font
#include <fonts/System6x7.h>
#include <fonts/SystemFont5x7.h>

// ID: Instansiasi Objek Utama Library
// EN: Main Library Object Instantiation
FansElectronics_DMD32 dmd;

// ID: KONFIGURASI PIN UNTUK TIPE HUB08 (P4.75 / P7.62 / Videotron)
// EN: PIN CONFIGURATION FOR HUB08 PANEL TYPE (P4.75 / P7.62 / Videotron)
DmdPins pinsHUB08= {
    .pin_A = 16,     // ID: Pin Alamat Baris A | EN: Scan Line Address A
    .pin_B = 14,     // ID: Pin Alamat Baris B | EN: Scan Line Address B
    .pin_C = 17,     // ID: Pin Alamat Baris C | EN: Scan Line Address C
    .pin_D = 5,     // ID: Pin Alamat Baris D | EN: Scan Line Address D
    .pin_latch = 19, // ID: STB / LATCH | EN: Latch strobe line
    .pin_sck = 18,   // ID: CLK / SCK | EN: Shift register clock line
    .pin_oe = 4,     // ID: nOE (Enable Output) | EN: Output Enable line
    .pin_mosi = 23, // ID: DATA Konektor Baris 1 | EN: DATA Connector Row 1
};
// ID: KONFIGURASI PIN UNTUK TIPE HUB12 (P10 Single/Multi-Panel)
// EN: PIN CONFIGURATION FOR HUB12 PANEL TYPE (P10 Single/Multi-Panel)
DmdPins pinsHUB12 = {
    .pin_A = 16,     // ID: Pin Alamat Baris A | EN: Scan Line Address A
    .pin_B = 14,     // ID: Pin Alamat Baris B | EN: Scan Line Address B
    .pin_C = -1,     // ID: Tidak dipakai di HUB12 | EN: Not used on HUB12
    .pin_D = -1,     // ID: Tidak dipakai di HUB12 | EN: Not used on HUB12
    .pin_latch = 19, // ID: STB / LATCH | EN: Latch strobe line
    .pin_sck = 18,   // ID: CLK / SCK | EN: Shift register clock line
    .pin_oe = 4,     // ID: nOE (Enable Output) | EN: Output Enable line
    .pin_mosi = 23, // ID: DATA Konektor Baris 1 | EN: DATA Connector Row 1
};

void setup()
{
    // -------------------------------------------------------------------------
    // ID: PANDUAN PENGUJIAN: Buka salah satu baris .begin() di bawah ini
    // EN: TESTING GUIDE: Uncomment one of the .begin() lines below
    // -------------------------------------------------------------------------

    // ID : Jika menggunakan 2 Panel HUB12 (P10) Jajar Horizontal (Resolusi 64 x 16 piksel)
    // EN : If using 2 HUB12 Panels (P10) Horizontal Array (Resolution
    dmd.begin(PANEL_HUB12, 2, 1, pinsHUB12);

    // ID : Jika menggunakan 2 Panel HUB08 Tunggal (Resolusi 128 x 16 piksel)
    // EN : If using 2 Single HUB08 Panels (Resolution 128 x 16 pixels)
    // dmd.begin(PANEL_HUB08, 2, 1, pinsHUB08);

    // -------------------------------------------------------------------------
    // ID: Mengaktifkan Double Buffer untuk render di latar belakang (Anti-Kedip)
    // EN: Enable Double Buffer for background rendering (Flicker-Free)
    dmd.setDoubleBuffer(true); // ID: Hapus jika tidak menggunakan double buffer | EN: Remove if not using double buffer

    // ID: Mengatur tingkat kecerahan LED (0 = Mati, 255 = Maksimal)
    // EN: Set LED brightness level (0 = Off, 255 = Maximum)
    dmd.setBrightness(128);
}

void loop()
{
    // ID: Bersihkan buffer memori latar belakang sebelum digambar ulang
    // EN: Clear background memory buffer before redrawing
    dmd.clear(); // ID: Hapus jika tidak menggunakan double buffer | EN: Remove if not using double buffer

    // ID: Atur font pertama dan cetak teks di baris atas
    // EN: Set first font and draw text on the top row
    dmd.setFont(System6x7);
    dmd.drawText(12, -1, "FansElectronics"); // ID: Koordinat X=2, Y=0 | EN: X=2, Y=0 Coordinates

    // ID: Atur font kedua dan cetak teks di baris bawah
    // EN: Set second font and draw text on the bottom row
    dmd.setFont(SystemFont5x7);
    dmd.drawText(12, 9, "DMD32 MULTI-PANEL"); // ID: Koordinat X=2, Y=8 | EN: X=2, Y=8 Coordinates

    // ID: Tukar buffer memori bayangan ke memori tampilan aktif secara instan
    // EN: Swap background memory buffer to active display memory instantly
    dmd.swapBuffers(); // ID: Hapus jika tidak menggunakan double buffer | EN: Remove if not using double buffer

    // ID: Trigger fungsi interupsi untuk memindai fisik baris LED via SPI
    // EN: Trigger interrupt function to scan physical LED rows via SPI
    dmd.update();
}