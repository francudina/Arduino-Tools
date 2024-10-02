#include <WiFiManager.h>

#ifdef USE_WIFI_MANAGER

void WiFiManager::initTime() {
    configTime(9 * 60 * 60, 0, "ntp.jst.mfeed.ad.jp", "ntp.nict.jp", "time.google.com");
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
        Serial.print("WiFiManager: Failed Connecting to WiFi");
        return false;
    }

    Serial.printf("WiFiManager: Connected with local IP: %s", 
        WiFi.localIP().toString().c_str());
    return true;
}

bool WiFiManager::isWiFiConnected() {
    return WiFi.status() == WL_CONNECTED;
}

bool WiFiManager::initVPN(
        const char* localIP, 
        const char* privateKey, 
        const char* remotePeerAddress, 
        const char* remotePeerPublicKey, 
        uint16_t remotePeerPort
) {
    // check connection
    if (!WiFiManager::isWiFiConnected()) {
        Serial.print("WiFiManager: WiFi isn't Connected");
        return false;
    }
    // configure time before
    WiFiManager::initTime();
    // init vpn client
    return wg.begin(
        IpUtils::ipFromString(localIP),     // IP address of the local interface
        privateKey,                         // Private key of the local interface
        remotePeerAddress,                  // Address of the endpoint peer.
        remotePeerPublicKey,                // Public key of the endpoint peer.
        remotePeerPort);                    // Port pf the endpoint peer.
}

#endif
