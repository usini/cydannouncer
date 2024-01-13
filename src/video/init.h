
#include <FS.h>
#include <SD.h>

/* variables */
static int next_frame = 0;
static int skipped_frames = 0;
static unsigned long start_ms, curr_ms, next_frame_ms;

#define FPS 30
#define MJPEG_BUFFER_SIZE (320 * 240 * 2 / 8)
#define AUDIOASSIGNCORE 1
#define DECODEASSIGNCORE 0
#define DRAWASSIGNCORE 1

#include <Arduino_GFX_Library.h>
#define GFX_BL TFT_BL // default backlight pin, you may replace DF_GFX_BL to actual backlight pin
Arduino_DataBus *bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCK, TFT_MOSI, TFT_MISO);
Arduino_GFX *gfx = new Arduino_ILI9341(bus, TFT_RST, 1 /* rotation */, false /* IPS */);

SPIClass spi = SPIClass(HSPI);

/* audio */
#include "video/esp32_audio_task.h"
/* MJPEG Video */
#include "video/mjpeg_decode_draw_task.h"

void screen_init()
{
#ifdef GFX_EXTRA_PRE_INIT
    GFX_EXTRA_PRE_INIT();
#endif

    Serial.println("Init display");
    if (!gfx->begin(80000000))
    {
        Serial.println("Init display failed!");
    }
    gfx->fillScreen(BLACK);

    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, HIGH);
}

void audio_init()
{
    Serial.println("Init I2S");
    gfx->println("Init I2S");

    esp_err_t ret_val = i2s_init(I2S_NUM_0, 44100, I2S_MCLK /* MCLK */, I2S_SCLK /* SCLK */, I2S_LRCK /* LRCK */, I2S_DOUT /* DOUT */, I2S_DIN /* DIN */);

    if (ret_val != ESP_OK)
    {
        Serial.printf("i2s_init failed: %d\n", ret_val);
    }
    i2s_zero_dma_buffer(I2S_NUM_0);
}

bool sd_init()
{
    Serial.println("Init FS");
    gfx->println("Init FS");

    spi.begin(SD_SCK, SD_MISO /* MISO */, SD_MOSI /* MOSI */, SD_CS /* SS */);
    bool status = !SD.begin(SD_CS /* SS */, spi, 80000000);
    return status;
}

