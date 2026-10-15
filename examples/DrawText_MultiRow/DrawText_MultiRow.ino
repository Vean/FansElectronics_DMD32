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

// ID: KONFIGURASI PIN UNTUK TIPE HUB12 MULTI-KONEKTOR (PARALEL)
// EN: PIN CONFIGURATION FOR HUB12 MULTI-CONNECTOR (PARALLEL MODE)

DmdPins pinsHUB12 = {
    .pin_A = 16,     // ID: Pin Alamat Baris A | EN: Scan Line Address A
    .pin_B = 14,     // ID: Pin Alamat Baris B | EN: Scan Line Address B
    .pin_C = -1,     // ID: Tidak dipakai di HUB12 | EN: Not used on HUB12
    .pin_D = -1,     // ID: Tidak dipakai di HUB12 | EN: Not used on HUB12
    .pin_latch = 19, // ID: STB / LATCH | EN: Latch strobe line
    .pin_sck = 18,   // ID: CLK / SCK | EN: Shift register clock line
    .pin_oe = 4,     // ID: nOE (Enable Output) | EN: Output Enable line
    .pin_data1 = 23, // ID: DATA Konektor Baris 1 | EN: DATA Connector Row 1
    .pin_data2 = 17, // ID: DATA Konektor Baris 2 | EN: DATA Connector Row 2
    .pin_data3 = 5,  // ID: DATA Konektor Baris 3 | EN: DATA Connector Row 3
    .pin_data4 = -1  // ID: DATA Konektor Baris 4 (Isi GPIO jika pakai 4 baris) | EN: DATA Connector Row 4
};


// ID: Atur jumlah baris fisik panel (2 atau 3 baris)
// EN: Set physical row count (2 or 3 rows)
const int ROW_COUNT = 3;

void setup()
{
    // ID: Inisialisasi: 1 Panel Lebar (32px) x ROW_COUNT Baris Tinggi, Mode SCAN_PARALLEL
    // EN: Initialization: 1 Panel Wide (32px) x ROW_COUNT Rows High, SCAN_PARALLEL Mode
    dmd.begin(PANEL_HUB12, 1, ROW_COUNT, pinsHUB12, SCAN_PARALLEL);

    // ID: Mengatur tingkat kecerahan LED (0 = Mati, 255 = Maksimal)
    // EN: Set LED brightness level (0 = Off, 255 = Maximum)
    dmd.setBrightness(128);
}

void loop()
{
    // ID: Pilih font teks monokrom
    // EN: Select monochrome text font
    dmd.setFont(SystemFont5x7);

    // ID: Cetak teks statis di Baris 1 (Y: 0 - 15)
    // EN: Draw static text on Row 1 (Y: 0 - 15)
    dmd.drawText(4, 4, "ROW 1");

    // ID: Cetak teks statis di Baris 2 (Y: 16 - 31) jika minimal 2 baris aktif
    // EN: Draw static text on Row 2 (Y: 16 - 31) if at least 2 rows active
    if (ROW_COUNT >= 2)
    {
        dmd.drawText(4, 20, "ROW 2");
    }

    // ID: Cetak teks statis di Baris 3 (Y: 32 - 47) jika 3 baris aktif
    // EN: Draw static text on Row 3 (Y: 32 - 47) if 3 rows active
    if (ROW_COUNT >= 3)
    {
        dmd.drawText(4, 36, "ROW 3");
    }

    // ID: Trigger pemindaian fisik baris LED
    // EN: Trigger interrupt function to scan physical LED rows
    dmd.update();
}