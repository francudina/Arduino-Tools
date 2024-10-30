#if defined(USE_WIREGUARD)
#include "WiFiManager.h"
#endif

#ifdef USE_WIREGUARD

void VpnManager::initTime() {
    configTime(9 * 60 * 60, 0, "ntp.jst.mfeed.ad.jp", "ntp.nict.jp", "time.google.com");
}

bool VpnManager::initWiFi(const char* ssid, const char* password, const int retry) {
    Serial.print("VpnManager: Connecting to WiFi...");
    // station mode set
    WiFi.mode(WIFI_STA);
    if (!WiFi.begin(ssid, password)) {
        Serial.print("VpnManager: Failed authorizing WiFi connection!");
        return false;
    }

    // connection retry
    int i = 0;
    while (!VpnManager::isWiFiConnected() && i++ <= retry) {
        Serial.print('.');
        delay(1000);
    }

    if (!VpnManager::isWiFiConnected()) {
        Serial.print("VpnManager: Failed Connecting to WiFi");
        return false;
    }

    Serial.printf("VpnManager: Connected with local IP: %s", 
        WiFi.localIP().toString().c_str());
    return true;
}

bool VpnManager::isWiFiConnected() {
    return WiFi.status() == WL_CONNECTED;
}

bool VpnManager::initVPN(
        const char* localIP, 
        const char* privateKey, 
        const char* remotePeerAddress, 
        const char* remotePeerPublicKey, 
        uint16_t remotePeerPort
) {
    // check connection
    if (!VpnManager::isWiFiConnected()) {
        Serial.print("VpnManager: WiFi isn't Connected");
        return false;
    }
    // configure time before
    VpnManager::initTime();
    // init vpn client
    return wg.begin(
        IpUtils::ipFromString(localIP),     // IP address of the local interface
        privateKey,                         // Private key of the local interface
        remotePeerAddress,                  // Address of the endpoint peer.
        remotePeerPublicKey,                // Public key of the endpoint peer.
        remotePeerPort);                    // Port pf the endpoint peer.
}

#endif
