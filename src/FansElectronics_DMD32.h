//----------------------------------------------------------------------------------------------
/*
 * Creator             : Irfan Indra Kurniawan, ST
 * Website             : https://www.fanselectronics.com
 * Buy Me a Coffee     : https://saweria.co/FansElectronics
 */

#ifndef FansElectronics_DMD32_h
#define FansElectronics_DMD32_h

#include "FansElectronics_Bitmap.h"
#include "SPI.h"
#include "esp_timer.h"
#include "Arduino.h"
#include "string.h"
#include "stdlib.h"

class FansElectronics_DMD32 : public FansElectronics_Bitmap
{
public:
    explicit FansElectronics_DMD32(int widthPanels = 1, int heightPanels = 1);
    ~FansElectronics_DMD32();
    bool doubleBuffer() const { return _doubleBuffer; }
    void setDoubleBuffer(bool doubleBuffer);
    void swapBuffers();
    void swapBuffersAndCopy();
    void begin();
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
    unsigned long lastRefresh;

    // ESP32 Specific handles
    SPIClass *dmdSPI;
    esp_timer_handle_t dispTimer;
};

#endif
//----------------------------------------------------------------------------------------------