//----------------------------------------------------------------------------------------------
/*
 * Creator             : Irfan Indra Kurniawan, ST
 * Website             : https://www.fanselectronics.com
 * Buy Me a Coffee     : https://saweria.co/FansElectronics
 * Ported for Multi-Panel Type (HUB08 & HUB12) on ESP32
 * Support P10, P4.25, P7.62, dan panel custom dengan konfigurasi pin fleksibel
 */

#ifndef FansElectronics_DMD32_h
#define FansElectronics_DMD32_h

#include "FansElectronics_Bitmap.h"
#include "SPI.h"
#include "esp_timer.h"
#include "soc/gpio_struct.h"
#include "driver/gpio.h"
#include "Arduino.h"
#include "string.h"
#include "stdlib.h"

// ID: Enumerasi tipe panel fisik matrix yang didukung
// EN: Enumeration of supported physical matrix panel types
enum PanelType
{
    PANEL_HUB08,
    PANEL_HUB12
};

// ID: Enumerasi metode pemindaian jalur data antar baris panel (Khusus HUB12)
// EN: Enumeration of data routing scan modes across panel rows (HUB12 Only)
enum ScanMode
{
    SCAN_ZIGZAG,   // Default: 1 Konektor, sambung seri/zigzag via Hardware SPI
    SCAN_PARALLEL  // Multi-Konektor: Tiap baris panel beda pin DATA (Max 4 Baris di ESP32)
};

// ID: Struktur data konfigurasi pin fleksibel untuk pemetaan GPIO ESP32 / ESP32-S3
// EN: Flexible pin configuration data structure for ESP32 / ESP32-S3 GPIO mapping
struct DmdPins
{
    int pin_A;     // Pin untuk alamat baris Multiplexing (Scan Line Addressing)
    int pin_B;     // Pin untuk alamat baris Multiplexing (Scan Line Addressing)
    int pin_C;     // ID: Diisi -1 jika HUB12 | EN: Set to -1 if HUB12
    int pin_D;     // ID: Diisi -1 jika HUB12 | EN: Set to -1 if HUB12
    int pin_latch; // LAT / SCLK
    int pin_sck;   // CLK
    int pin_oe;    // OE / EN
    union
    {
        int pin_data1; // DATA Konektor Baris 1 (Utama / R1)
        int pin_mosi;  // Alias untuk kompatibilitas kode lama
    };
    int pin_data2; // DATA Konektor Baris 2 (Diisi -1 jika tidak dipakai)
    int pin_data3; // DATA Konektor Baris 3 (Diisi -1 jika tidak dipakai)
    int pin_data4; // DATA Konektor Baris 4 (Khusus ESP32, diisi -1 jika tidak dipakai)
};
class FansElectronics_DMD32 : public FansElectronics_Bitmap
{
public:
    // ID: Konstruktor dasar untuk alokasi memori dinamis
    // EN: Default constructor for dynamic memory allocation
    FansElectronics_DMD32();
    ~FansElectronics_DMD32();

    bool doubleBuffer() const { return _doubleBuffer; }
    void setDoubleBuffer(bool doubleBuffer);
    void swapBuffers();
    void swapBuffersAndCopy();

    // ID: Inisialisasi utama driver layar dengan parameter dimensi & mode scan fleksibel
    // EN: Core display driver initialization function with flexible dimension & scan mode parameters
    void begin(PanelType type, int panelsWide, int panelsHigh, DmdPins pins, int panelWidthPx = 0, int panelHeightPx = 0, ScanMode scanMode = SCAN_ZIGZAG);

    // ID: Overload fungsi begin() agar mudah memanggil ScanMode tanpa harus mengisi resolusi piksel
    // EN: Overloaded begin() helper to easily pass ScanMode without specifying custom pixel dimensions
    void begin(PanelType type, int panelsWide, int panelsHigh, DmdPins pins, ScanMode scanMode)
    {
        begin(type, panelsWide, panelsHigh, pins, 0, 0, scanMode);
    }

    void update();
    void refresh();
    void setBrightness(uint8_t b);
    static Color fromRGB(uint8_t r, uint8_t g, uint8_t b);

private:
    FansElectronics_DMD32(const FansElectronics_DMD32 &other) : FansElectronics_Bitmap(other) {}
    FansElectronics_DMD32 &operator=(const FansElectronics_DMD32 &) { return *this; }
    uint8_t brightenss;
    bool _doubleBuffer;
    uint8_t phase;
    uint8_t *fb0;
    uint8_t *fb1;
    uint8_t *displayfb;

    // ID: Variabel pelacak konfigurasi internal driver
    // EN: Internal driver configuration tracking variables
    PanelType _panelType;
    ScanMode _scanMode;
    DmdPins _pins;
    int _panelsWide;
    int _panelsHigh;
    int _panelWidthPx;
    int _panelHeightPx;

    // Bitmask cepat untuk GPIO Bank 0 (GPIO 0..31) dan Bank 1 (GPIO 32+)
    bool _useFastGpioBank0;
    uint32_t _maskD0;
    uint32_t _maskD1;
    uint32_t _maskD2;
    uint32_t _maskD3;
    uint32_t _maskCLK;
    uint32_t _maskUnusedData;

    SPIClass *dmdSPI;
    esp_timer_handle_t timerHandle;

    // ID: Fungsi internal pengirim 1 byte data paralel (1, 2, 3, atau 4 baris sekaligus)
    // EN: Internal fast bit-banging helper for parallel multi-connector row shifting
    inline void shiftOutParallel1(uint8_t b0);
    inline void shiftOutParallel2(uint8_t b0, uint8_t b1);
    inline void shiftOutParallel3(uint8_t b0, uint8_t b1, uint8_t b2);
    inline void shiftOutParallel4(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3);
};

#endif
//----------------------------------------------------------------------------------------------