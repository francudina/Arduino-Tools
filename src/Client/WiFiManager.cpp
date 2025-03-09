#if defined(USE_WIFI)
#include "WiFiManager.h"

void WiFiManager::initTime() {
    Serial.println("WiFiManager: Time Init ...");
    configTime(9 * 60 * 60, 0, "ntp.jst.mfeed.ad.jp", "ntp.nict.jp", "time.google.com");
    Serial.println("WiFiManager: Time Init: done");
}

bool WiFiManager::initWiFi(const char* ssid, const char* password, const int retry) {
    Serial.print("WiFiManager: Connecting to WiFi...");
    if (WiFiManager::isWiFiConnected()) {
        Serial.println("WiFiManager: WiFi already connected!");
        return true;
    }
    // station mode set
    WiFi.mode(WIFI_STA);
    if (!WiFi.begin(ssid, password)) {
        Serial.print("WiFiManager: Failed authorizing WiFi connection!");
        return false;
    }

    // connection retry
    int i = 0;
    while (!WiFiManager::isWiFiConnected() && i++ <= retry) {
        Serial.print('.');
        delay(1000);
    }

    if (!WiFiManager::isWiFiConnected()) {
        Serial.println("\nWiFiManager: Failed Connecting to WiFi");
        return false;
    }

    Serial.printf("\nWiFiManager: Connected with local IP: %s\n", 
        WiFi.localIP().toString().c_str());
    return true;
}

bool WiFiManager::disconnect() {
    return WiFi.disconnect();
}

bool WiFiManager::isWiFiConnected() {
    return WiFi.status() == WL_CONNECTED;
}

#endif
