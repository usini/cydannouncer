// Based on https://github.com/moononournation/MiniTV

#include <Arduino.h>
#include <WiFi.h>
#include <ezTime.h>

#include "pinout.h"
#include "media.h"
#include "internet.h"
#include "clock.h"



void setup()
{
  Serial.begin(115200);
  media_init();
  // gfx->println("Connexion au WiFi");


  drawJPGpos(256, 0, "/nowifi.jpg");
  start_wifi();
  // play_video("/bleed");
}

void loop()
{
  display_clock();
  //
  // Serial.println(UTC.dateTime());
  // gfx->println(UTC.dateTime());
  // delay(1000);
}
