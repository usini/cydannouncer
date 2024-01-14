String wifi_ssid = "";
String wifi_password = "";
Timezone myTz;

void WiFiTask(void *parameter)
{
    WiFi.begin(wifi_ssid, wifi_password);
    Serial.println("Connexion a " + wifi_ssid);
    while (WiFi.status() != WL_CONNECTED)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    Serial.println("Connecte au WiFi");
    while (!screen_available)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    drawJPGpos(256, 0, "/wifi.jpg");
    myTz.setLocation("Europe/Paris");
    waitForSync();
    while (1)
    {
        events();
    }
}

bool read_wifi_credentials()
{
    File file = SD.open("/wifi.txt", "r");
    if (!file)
    {
        Serial.println("wifi.txt is not present");
        gfx->println("wifi.txt absent");
        return false;
    }
    wifi_ssid = file.readStringUntil('\n');
    wifi_ssid.trim();

    wifi_password = file.readStringUntil('\n');
    wifi_password.trim();
    Serial.println("Wifi Credentials read");
    file.close();
    return true;
}

bool start_wifi()
{
    if (read_wifi_credentials())
    {
        xTaskCreate(
            WiFiTask,   // Fonction pour la tâche
            "WiFiTask", // Nom de la tâche
            3000,      // Taille de la pile pour la tâche
            NULL,       // Paramètre pour la tâche
            0,          // Priorité de la tâche
            NULL        // Handle de la tâche
        );
        return true;
    }
    else
    {
        return false;
    }
}