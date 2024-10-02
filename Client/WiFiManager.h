#pragma once

#ifdef USE_WIFI_MANAGER

#include <WiFi.h>
#include <WireGuard-ESP32.h>

#include "NetworkUtils.h"


class WiFiManager {
private:
    static WireGuard wg;

public:
    static bool initWiFi(const char* ssid, const char* pass, const int retry = 10);

    static bool isWiFiConnected();

    static bool initVPN(
        const char* localIP, 
        const char* privateKey, 
        const char* remotePeerAddress, 
        const char* remotePeerPublicKey, 
        uint16_t remotePeerPort);

    static void initTime();
};

#endif
