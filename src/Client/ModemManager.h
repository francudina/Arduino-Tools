#pragma once

#include <vector>

#include <WiFi.h>
#include <Arduino.h>
#include <TinyGsmClient.h>

#include "NetworkUtils.h"

#ifndef MODEM_CONFIG
// Set serial for AT commands (to the module)
#define SerialAT Serial1

// Select your modem
// #define TINY_GSM_MODEM_SIM7600
#ifndef TINY_GSM_MODEM_SIM7600
#define TINY_GSM_MODEM_SIM7600
#endif

// LilyGO T-SIM7000G Pinout
// T-A7670-ESP32  DeepSleep ~ 157 uA
#define MODEM_BAUD    115200
#define MODEM_DTR_PIN 25
#define MODEM_TX_PIN  26
#define MODEM_RX_PIN  27
#define MODEM_PWR_PIN 4

// Increase RX buffer to capture the entire response
// Chips without internal buffering (A6/A7, ESP8266, M590)
// need enough space in the buffer for the entire response
// else data will be lost (and the http library will fail).
#ifndef TINY_GSM_RX_BUFFER
#define TINY_GSM_RX_BUFFER 1024
#endif

// Define how you're planning to connect to the internet
// These defines are only for this example; they are not needed in other code.
// GPRS (General Packet Radio Service)
#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false

// set GSM PIN, if any
#define GSM_PIN ""

// See all AT commands, if wanted
// #define DUMP_AT_COMMANDS

// Add a reception delay, if needed.
// This may be needed for a fast processor at a slow baud rate.
// #define TINY_GSM_YIELD() { delay(2); }
#endif

#ifndef NETWORK_RETRIES_NUM
#define NETWORK_RETRIES_NUM 1000
#endif

// Your WiFi connection credentials, if applicable
#ifndef WIFI_CONFIG
#define wifi_ssid ""
#define wifi_pass ""
#endif

// Your GPRS credentials, if any
#ifndef GPRS_CONFIG
#define apn_name    "internet.ht.hr"
#define gprs_user   ""
#define gprs_pass   ""
#endif

// Access Point (AP) aka Hotspot
// - network config
#ifndef AP_CONFIG
#define ap_local_ip    "192.168.2.1"
#define ap_gateway_ip  "192.168.2.1"
#define ap_subnet_mask "255.255.255.0"
#endif

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
