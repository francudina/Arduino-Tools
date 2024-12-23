#pragma once

#if defined(USE_WIFI)
#include <WiFi.h>

#include "../Utils/NetworkUtils.h"


class WiFiManager {
public:
    static bool initWiFi(const char* ssid, const char* pass, const int retry = 10);

    static bool isWiFiConnected();

    static void initTime();
};

#endif
