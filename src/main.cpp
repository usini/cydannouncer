#include <Arduino.h>
#include <WiFi.h>

// Based on https://github.com/moononournation/MiniTV

#include "pinout.h"
#include "video/init.h"
#include "video/functions.h"

void setup()
{
  WiFi.mode(WIFI_OFF);

  Serial.begin(115200);
  Serial.println("MJPEG_2task_Audio_1task");

  screen_init();
  audio_init();
  if (sd_init())
  {
    Serial.println("ERROR: File system mount failed!");
    gfx->println("ERROR: File system mount failed!");
  }
  else
  {
    open_video("/bleed");
  }
}

void loop()
{
}
