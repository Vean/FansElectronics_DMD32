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
#include "Arduino.h"
#include "string.h"
#include "stdlib.h"

// Enumerasi Tipe Panel
enum PanelType
{
    PANEL_HUB08,
    PANEL_HUB12
};

// Struktur Data Pin Konfigurasi Dinamis
struct DmdPins
{
    int pin_A;
    int pin_B;
    int pin_C;     // Diisi -1 jika HUB12
    int pin_D;     // Diisi -1 jika HUB12
    int pin_latch; // LAT / SCLK
    int pin_sck;   // CLK
    int pin_oe;    // OE / EN
    int pin_mosi;  // DATA / R1
};

class FansElectronics_DMD32 : public FansElectronics_Bitmap
{
public:
    // Constructor default kosong (ramah pemula, alokasi memori dipindah ke begin)
    FansElectronics_DMD32();
    ~FansElectronics_DMD32();

    bool doubleBuffer() const { return _doubleBuffer; }
    void setDoubleBuffer(bool doubleBuffer);
    void swapBuffers();
    void swapBuffersAndCopy();

    // Fungsi begin baru yang fleksibel
    void begin(PanelType type, int panelsWide, int panelsHigh, DmdPins pins, int panelWidthPx = 0, int panelHeightPx = 0);

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

    // Variabel Konfigurasi Internal Library
    PanelType _panelType;
    DmdPins _pins;
    int _panelsWide;
    int _panelsHigh;
    int _panelWidthPx;
    int _panelHeightPx;

    SPIClass *dmdSPI;
    esp_timer_handle_t timerHandle;
};

#endif
//----------------------------------------------------------------------------------------------