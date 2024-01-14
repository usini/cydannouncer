#include "FreeSans24pt7b.h"

void display_clock()
{
  String heure = myTz.dateTime("H:i:s"); // Récupérer l'heure au format souhaité

  int16_t x1, y1;
  uint16_t w, h;
  gfx->setFont(&FreeSans24pt7b);
  gfx->setTextColor(WHITE);                          // Couleur du texte
  gfx->getTextBounds(heure, 0, 0, &x1, &y1, &w, &h); // Calculer la taille du texte

  //int x = (320 - w) / 2; // Calculer la position X pour centrer
  //int y = (240 - h) / 2; // Calculer la position Y pour centrer
  //Serial.println(x);
  //Serial.println(y);
  gfx->fillRect(73, 63, 192, 48, BLACK);
  gfx->setCursor(73, 103); // Définir la position du curseur

  gfx->print(heure); // Afficher l'heure

  delay(1000); // Mettre à jour l'heure toutes les secondes
}