// Based on https://github.com/moononournation/MiniTV

#include <Arduino.h>
#include <WiFi.h>

#include "pinout.h"
#include "media.h"

void setup()
{
  WiFi.mode(WIFI_OFF);

  Serial.begin(115200);

  media_init();
  play_video("/bleed");
}

void loop()
{
}
