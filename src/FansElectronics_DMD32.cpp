//----------------------------------------------------------------------------------------------
/*
 * Creator             : Irfan Indra Kurniawan, ST
 * Website             : https://www.fanselectronics.com
 * Buy Me a Coffee     : https://saweria.co/FansElectronics
 */

#include "FansElectronics_DMD32.h"

// Flags interupsi timer
volatile bool tickOccured = false;

// Pemetaan PIN Baru untuk ESP32-S3 sesuai permintaan
#define DMD3_PIN_A 48
#define DMD3_PIN_B 47
#define DMD3_PIN_C 21
#define DMD3_PIN_D 14
#define DMD3_PIN_LATCH 13         // LAT
#define DMD3_PIN_SPI_SCK 12       // CLK
#define DMD3_PIN_OUTPUT_ENABLE 11 // OE
#define DMD3_PIN_SPI_MOSI 10      // R1
#define DMD3_PIN_SPI_MISO -1      // Tidak digunakan (HUB08 bersifat Write-Only)

#define DMD3_NUM_COLUMNS 64
#define DMD3_NUM_ROWS 16
#define DMD3_REFRESH_US 100

// Callback timer versi ESP32 (menggunakan IRAM_ATTR agar berjalan di RAM)
void IRAM_ATTR timerCallback(void *pArg)
{
    tickOccured = true;
}

//----------------------------------------------------------------------------------------------
FansElectronics_DMD32::FansElectronics_DMD32(int widthPanels, int heightPanels)
    : FansElectronics_Bitmap(widthPanels * DMD3_NUM_COLUMNS, heightPanels * DMD3_NUM_ROWS),
      _doubleBuffer(false), phase(0), fb0(0), fb1(0), displayfb(0), lastRefresh(millis()),
      dmdSPI(NULL), dispTimer(NULL)
{
    fb0 = displayfb = fb;
    brightenss = 255; // Default full brightness
}

//----------------------------------------------------------------------------------------------
FansElectronics_DMD32::~FansElectronics_DMD32()
{
    if (dispTimer)
    {
        esp_timer_stop(dispTimer);
        esp_timer_delete(dispTimer);
    }
    if (dmdSPI)
    {
        dmdSPI->end();
        delete dmdSPI;
    }
    if (fb0)
        free(fb0);
    if (fb1)
        free(fb1);
    fb = 0;
}

//----------------------------------------------------------------------------------------------
void FansElectronics_DMD32::begin()
{
    // Konfigurasi GPIO Control Panel
    pinMode(DMD3_PIN_A, OUTPUT);
    pinMode(DMD3_PIN_B, OUTPUT);
    pinMode(DMD3_PIN_C, OUTPUT);
    pinMode(DMD3_PIN_D, OUTPUT);
    pinMode(DMD3_PIN_OUTPUT_ENABLE, OUTPUT);
    pinMode(DMD3_PIN_LATCH, OUTPUT);

    digitalWrite(DMD3_PIN_A, LOW);
    digitalWrite(DMD3_PIN_B, LOW);
    digitalWrite(DMD3_PIN_C, LOW);
    digitalWrite(DMD3_PIN_D, LOW);
    digitalWrite(DMD3_PIN_OUTPUT_ENABLE, HIGH); // OE aktif biasanya LOW, set HIGH untuk mematikan display saat start
    digitalWrite(DMD3_PIN_LATCH, LOW);

    // Inisialisasi Hardware SPI pada ESP32-S3 (Menggunakan FSPI / HSPI secara bebas)
    dmdSPI = new SPIClass(HSPI);
    // Format begin: sck, miso, mosi, ss (-1 karena LATCH diatur manual)
    dmdSPI->begin(DMD3_PIN_SPI_SCK, DMD3_PIN_SPI_MISO, DMD3_PIN_SPI_MOSI, -1);

    // Setup High-Resolution Timer milik ESP32 SDK
    tickOccured = false;
    const esp_timer_create_args_t timer_args = {
        .callback = &timerCallback,
        .arg = NULL,
        .name = "dmd_refresh_timer"};
    esp_timer_create(&timer_args, &dispTimer);
    // Jalankan timer secara periodik dalam satuan mikrodetik (us)
    esp_timer_start_periodic(dispTimer, DMD3_REFRESH_US);
}

//----------------------------------------------------------------------------------------------
void FansElectronics_DMD32::setDoubleBuffer(bool doubleBuffer)
{
    if (doubleBuffer != _doubleBuffer)
    {
        _doubleBuffer = doubleBuffer;
        if (doubleBuffer)
        {
            unsigned int size = _stride * _height;
            fb1 = (uint8_t *)malloc(size);
            if (fb1)
            {
                memset(fb1, 0xFF, size);
                noInterrupts(); // Pengganti cli() di ESP32
                fb = fb1;
                displayfb = fb0;
                interrupts(); // Pengganti sei() di ESP32
            }
            else
            {
                _doubleBuffer = false;
            }
        }
        else if (fb1)
        {
            noInterrupts();
            fb = fb0;
            displayfb = fb0;
            interrupts();
            free(fb1);
            fb1 = 0;
        }
    }
}

//----------------------------------------------------------------------------------------------
void FansElectronics_DMD32::update()
{
    if (tickOccured == true)
    {
        tickOccured = false;
        refresh();
    }
}

//----------------------------------------------------------------------------------------------
void FansElectronics_DMD32::refresh()
{
    int stride16 = _stride * 16;
    volatile uint8_t *data0;

    // Memulai Transaksi SPI (Frekuensi 10MHz, MSBFIRST, SPI_MODE0)
    dmdSPI->beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));

    for (int y = 0; y < _height; y += 32)
    {
        data0 = displayfb + _stride * (y + phase);
        for (int x = _stride; x > 0; --x)
        {
            // Kirim data terbalik (~*data0) sesuai dengan logika panel HUB08 bawaan library asli
            dmdSPI->write(~*data0++);
        }
        data0 = data0 + stride16;
    }

    // Akhiri Transaksi SPI
    dmdSPI->endTransaction();

    // Latch data (Kunci data ke register shift panel)
    digitalWrite(DMD3_PIN_OUTPUT_ENABLE, HIGH); // Matikan display sebentar untuk cegah ghosting

    digitalWrite(DMD3_PIN_LATCH, HIGH);
    digitalWrite(DMD3_PIN_LATCH, LOW);

    // Atur alamat baris Multiplexing (A,B,C,D) → 16 fase
    digitalWrite(DMD3_PIN_A, (phase & 0x01) ? HIGH : LOW);
    digitalWrite(DMD3_PIN_B, (phase & 0x02) ? HIGH : LOW);
    digitalWrite(DMD3_PIN_C, (phase & 0x04) ? HIGH : LOW);
    digitalWrite(DMD3_PIN_D, (phase & 0x08) ? HIGH : LOW);

// Konfigurasi LEDC PWM untuk pin OE kustom
// Sesuai Arduino Core ESP32 versi baru, ledcSetup dilewati langsung mengikat pin dan channel
#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
    ledcAttach(DMD3_PIN_OUTPUT_ENABLE, 10000, 8); // Channel otomatis diatur oleh Core v3
    ledcWrite(DMD3_PIN_OUTPUT_ENABLE, 255 - brightenss);
#else
    ledcSetup(0, 10000, 8);
    ledcAttachPin(DMD3_PIN_OUTPUT_ENABLE, 0);
    ledcWrite(0, 255 - brightenss);
#endif

    // Naikkan phase (0..15) kemudian looping balik ke 0
    phase = (phase + 1) & 0x0F;
}

//----------------------------------------------------------------------------------------------
void FansElectronics_DMD32::setBrightness(uint8_t b)
{
    // Batasi ke rentang PWM standard (0-255)
    brightenss = b;
}

//----------------------------------------------------------------------------------------------
FansElectronics_DMD32::Color FansElectronics_DMD32::fromRGB(uint8_t r, uint8_t g, uint8_t b)
{
    if (r || g || b)
    {
        return White;
    }
    else
    {
        return Black;
    }
}

//----------------------------------------------------------------------------------------------
void FansElectronics_DMD32::swapBuffers()
{
    if (_doubleBuffer)
    {
        noInterrupts();
        if (fb == fb0)
        {
            fb = fb1;
            displayfb = fb0;
        }
        else
        {
            fb = fb0;
            displayfb = fb1;
        }
        interrupts();
    }
}

//----------------------------------------------------------------------------------------------
void FansElectronics_DMD32::swapBuffersAndCopy()
{
    swapBuffers();
    if (_doubleBuffer)
        memcpy(fb, displayfb, _stride * _height);
}
//----------------------------------------------------------------------------------------------