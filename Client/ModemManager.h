#pragma once

#ifdef USE_MODEM_MANAGER

#include <vector>

#include <WiFi.h>
#include <Arduino.h>
#include <TinyGsmClient.h>

#include "NetworkUtils.h"

#include "../../src/config.h"

class ModemManager {
public:
    ModemManager();
    TinyGsmClient& getGsmClient();

    bool modemInit();
    bool connect();

    bool connectToNetwork(const char* wifiSSID, const char* wifiPass, const char* apn, const char* gprsUser, const char* gprsPass);
    void disconnectFromNetwork();

    bool enterSleepMode();
    bool isModemOnline(const int retry = 10);

    bool isConnected();
    int getSignalQuality();
    String getIpAddress();
    String getModemName();
    String getModemInfo();
    String getNetworkMode();

    RegStatus getRegistrationStatus(const bool debug = false);
    void printRegistrationStatus();

    bool setAccessPoint(const char* ssid, const char* pass);
    bool forwardIncomingRequests();

    IPAddress getAccessPointIP();
    uint8_t numOfAccessPointClients();
    std::vector<String> accessPointClientsConnected();

private:
    TinyGsm modem;

    bool waitForNetwork();
    bool wakeupFromSleepMode();
};

#endif
