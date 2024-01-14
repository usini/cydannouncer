// Based on https://github.com/moononournation/MiniTV

#include <Arduino.h>
#include <WiFi.h>
#include <ezTime.h>

#include "pinout.h"
#include "media.h"

void WiFiTask(void *parameter)
{
  WiFi.begin("", "");
  Serial.println("Connexion au WiFi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    vTaskDelay(pdMS_TO_TICKS(100));
  }
  Serial.println("Connecte au WiFi");
  gfx->println("Connecte au WiFi");
  while (!screen_available)
  {
    vTaskDelay(pdMS_TO_TICKS(10));
  }
  drawJPGpos(256, 0, "/wifi.jpg");
  gfx->println("Connexion au serveur de temps");
  Timezone myTz;
  myTz.setLocation();
  waitForSync();
  gfx->println("Connecte au serveur de temps");
  vTaskDelete(NULL);
}

void start_wifi()
{
  xTaskCreate(
      WiFiTask,   // Fonction pour la tâche
      "WiFiTask", // Nom de la tâche
      10000,      // Taille de la pile pour la tâche
      NULL,       // Paramètre pour la tâche
      0,          // Priorité de la tâche
      NULL        // Handle de la tâche
  );
}

void setup()
{
  Serial.begin(115200);
  media_init();
  gfx->println("Connexion au WiFi");

  drawJPGpos(288, 0, "/nosdcard.jpg");
  drawJPGpos(256, 0, "/nowifi.jpg");
  start_wifi();
  // play_video("/bleed");
}

void loop()
{
  // events();
  // Serial.println(UTC.dateTime());
  // gfx->println(UTC.dateTime());
  // delay(1000);
}
