/*
 * Creator             : Irfan Indra Kurniawan, ST
 * Website             : https://www.fanselectronics.com
 * Buy Me a Coffee     : https://saweria.co/FansElectronics
 * Tested on ESP32-S3 with HUB08 and HUB12 Panels
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

// =========================================================================
// 1. ID: KONFIGURASI PIN UNTUK TIPE HUB08 (P4.75 / P7.62 / Videotron)
//    EN: PIN CONFIGURATION FOR HUB08 PANEL TYPE (P4.75 / P7.62 / Videotron)
// =========================================================================
DmdPins pinsHUB08 = {
    .pin_A = 48,
    .pin_B = 47,
    .pin_C = 21,
    .pin_D = 14,
    .pin_latch = 13,
    .pin_sck = 12,
    .pin_oe = 11,
    .pin_mosi = 10};

// =========================================================================
// 2. ID: KONFIGURASI PIN UNTUK TIPE HUB12 (P10 Single/Multi-Panel)
//    EN: PIN CONFIGURATION FOR HUB12 PANEL TYPE (P10 Single/Multi-Panel)
// =========================================================================
DmdPins pinsHUB12 = {
    .pin_A = 47,
    .pin_B = 21,
    .pin_C = -1, // ID: HUB12 tidak menggunakan pin C | EN: HUB12 does not use pin C
    .pin_D = -1, // ID: HUB12 tidak menggunakan pin D | EN: HUB12 does not use pin D
    .pin_latch = 13,
    .pin_sck = 14,
    .pin_oe = 48,
    .pin_mosi = 12};

void setup()
{
    // -------------------------------------------------------------------------
    // ID: PANDUAN PENGUJIAN: Buka salah satu baris .begin() di bawah ini
    // EN: TESTING GUIDE: Uncomment one of the .begin() lines below
    // -------------------------------------------------------------------------

    // ID : Jika menggunakan 2 Panel HUB08 Tunggal (Resolusi 128 x 16 piksel)
    // EN : If using 2 Single HUB08 Panels (Resolution 128 x 16 pixels)
    dmd.begin(PANEL_HUB08, 2, 1, pinsHUB08);

    // ID : Jika menggunakan 2 Panel HUB12 (P10) Jajar Horizontal (Resolusi 64 x 16 piksel)
    // EN : If using 2 HUB12 Panels (P10) Horizontal Array (Resolution
    // dmd.begin(PANEL_HUB12, 2, 1, pinsHUB12);

    // -------------------------------------------------------------------------

    // ID: Mengaktifkan Double Buffer untuk render di latar belakang (Anti-Kedip)
    // EN: Enable Double Buffer for background rendering (Flicker-Free)
    dmd.setDoubleBuffer(true);

    // ID: Mengatur tingkat kecerahan LED (0 = Mati, 255 = Maksimal)
    // EN: Set LED brightness level (0 = Off, 255 = Maximum)
    dmd.setBrightness(128);
}

void loop()
{
    // 1. ID: Bersihkan buffer memori latar belakang sebelum digambar ulang
    //    EN: Clear background memory buffer before redrawing
    dmd.clear();

    // 2. ID: Atur font pertama dan cetak teks di baris atas
    //    EN: Set first font and draw text on the top row
    dmd.setFont(System6x7);
    dmd.drawText(12, 0, "FansElectronics");

    // 3. ID: Jalankan teks berjalan di baris bawah (Y=8)
    //    EN: Execute running text on the bottom row (Y=8)
    dmd.setFont(SystemFont5x7);
    runningText(9, 30, "FansElectronics_DMD32 Dotmatrix on ESP32 - Multi-Panel Supported! ");

    // 4. ID: Tukar buffer memori bayangan ke memori tampilan aktif secara instan
    //    EN: Swap background memory buffer to active display memory instantly
    dmd.swapBuffers();

    // 5. ID: Trigger fungsi interupsi untuk memindai fisik baris LED via SPI
    //    EN: Trigger interrupt function to scan physical LED rows via SPI
    dmd.update();
}

// =========================================================================
// ID: FUNGSI PEMBANTU: RUNNING TEXT (ANIMASI TEKS BERJALAN)
// EN: HELPER FUNCTION: RUNNING TEXT (TEXT ANIMATION)
// =========================================================================
void runningText(int y, uint8_t speed, String text)
{
    static uint32_t lastScrollTime;
    static int currentX = 0;

    if ((millis() - lastScrollTime) > speed)
    {
        lastScrollTime = millis();
        currentX++;
        if (currentX > dmd.textWidth(text))
        {
            currentX = 0; // ID: Reset ke awal jika teks sudah habis | EN: Reset to start when text finished
        }
    }
    dmd.drawText(dmd.width() - currentX, y, text);
}