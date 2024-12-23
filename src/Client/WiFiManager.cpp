#if defined(USE_WIFI)
#include "WiFiManager.h"

void WiFiManager::initTime() {
    Serial.println("WiFiManager: Time Init ...");
    configTime(9 * 60 * 60, 0, "ntp.jst.mfeed.ad.jp", "ntp.nict.jp", "time.google.com");
    Serial.println("WiFiManager: Time Init: done");
}

bool WiFiManager::initWiFi(const char* ssid, const char* password, const int retry) {
    Serial.print("WiFiManager: Connecting to WiFi...");
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
        Serial.print("\nWiFiManager: Failed Connecting to WiFi");
        return false;
    }

    Serial.printf("\nWiFiManager: Connected with local IP: %s", 
        WiFi.localIP().toString().c_str());
    return true;
}

bool WiFiManager::isWiFiConnected() {
    return WiFi.status() == WL_CONNECTED;
}

#endif
