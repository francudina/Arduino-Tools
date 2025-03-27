#ifdef USE_GSM_MODEM

#include "ModemManager.h"

ModemManager::ModemManager() : modem(SerialAT) { }

/*
    Example: https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/blob/main/examples/ModemSleep/ModemSleep.ino
*/
bool ModemManager::modemInit() {
    Serial.println("Modem: modem init...");
    SerialAT.begin(MODEM_BAUD, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);

    bool wakeUpCause = esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER;
    Serial.printf("Modem: Woken up with timer from sleep: %s\n", wakeUpCause ? "TRUE" : "FALSE");

//     // Turn on DC boost to power on the modem
#ifdef BOARD_POWERON_PIN
    pinMode(BOARD_POWERON_PIN, OUTPUT);
    digitalWrite(BOARD_POWERON_PIN, HIGH);
    Serial.println("Modem: Powering on the modem through POWERON pin");
#endif

    delay(2000);
#ifdef MODEM_RESET_PIN
    // Release reset GPIO hold
    gpio_hold_dis((gpio_num_t)MODEM_RESET_PIN);

    // Set modem reset pin ,reset modem
    // The module will also be started during reset.
    Serial.println("Modem: Setting Reset Pin");
    pinMode(MODEM_RESET_PIN, OUTPUT);
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL); delay(100);
    digitalWrite(MODEM_RESET_PIN, MODEM_RESET_LEVEL); delay(2600);
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL);
#endif

    pinMode(MODEM_DTR_PIN, OUTPUT);
    digitalWrite(MODEM_DTR_PIN, LOW);

    Serial.println("Modem: Power on the modem PWRKEY.");
    pinMode(BOARD_PWRKEY_PIN, OUTPUT);
    digitalWrite(BOARD_PWRKEY_PIN, LOW);
    delay(100);
    digitalWrite(BOARD_PWRKEY_PIN, HIGH);
    //Ton >= 100 <= 500
    delay(300);
    digitalWrite(BOARD_PWRKEY_PIN, LOW);

    // Pull up DTR to put the modem into sleep
    pinMode(MODEM_DTR_PIN, OUTPUT);
    digitalWrite(MODEM_DTR_PIN, HIGH);

    delay(2000);

    Serial.println("Modem: Check if modem is online...");
    while (!modem.testAT()) {
        delay(500);
    }
    bool modemBoot = modem.testAT(1000);
    Serial.printf("Modem: Modem is %s\n", modemBoot ? "ONLINE" : "OFFLINE");

    Serial.printf("Modem: modem init%s: %s\n",
        wakeUpCause ? " WAKEUP from sleep" : "",
        modemBoot ? "success" : "fail");
    
#ifdef DEVICE_LOGGING
    // modem info
    String name = getModemName();
    String modemInfo = getModemInfo();
    Serial.println("Modem: Name: " + name);
    Serial.println("Modem: Info: " + modemInfo);
#endif

    return modemBoot;
}

bool ModemManager::connect() {
    int tryCount = 0;
    while (++tryCount < NETWORK_RETRIES_NUM) {
        bool connected = connectToNetwork(wifi_ssid, wifi_pass, apn_name, gprs_user, gprs_pass);
        if (connected) { break; }

        Serial.printf("Modem: Failed to connect to network. Retrying... (%d)\n", tryCount);
        delay(1000);
    }
    return tryCount < NETWORK_RETRIES_NUM;
}

bool ModemManager::connectToNetwork(
    const char* wifiSSID = "",
    const char* wifiPass = "",
    const char* apn = "", 
    const char* gprsUser = "", 
    const char* gprsPass = ""
) {
    Serial.println("Modem: Connecting to network...");

// GPRS (General Packet Radio Service)
#if TINY_GSM_USE_GPRS
    // Unlock your SIM card with a PIN if needed
    if (GSM_PIN && modem.getSimStatus() != 3 ) {
        bool unlocked = modem.simUnlock(GSM_PIN);
        Serial.printf("Modem: SIM unlocked: %s\n", unlocked ? "true" : "false");
    }
#endif

    bool fail = false;
#if TINY_GSM_USE_WIFI
    // Wifi connection parameters must be set before waiting for the network
    Serial.println(F("Modem: Setting WiFi SSID/password..."));
    if (!modem.networkConnect(wifiSSID, wifiPass)) { fail = true; }
#endif

    Serial.println("Modem: Waiting for network...");
    if (!modem.waitForNetwork()) { fail = true; }
    if (modem.isNetworkConnected()) { Serial.println("Modem: Network connected"); }

#if TINY_GSM_USE_GPRS
    // GPRS connection parameters are usually set after network registration
    Serial.printf("Modem: Connecting to APN: %s\n", apn);
    if (!modem.gprsConnect(apn, gprsUser, gprsPass)) { fail = true; }
    if (modem.isGprsConnected()) { Serial.println("Modem: GPRS connected"); }
#endif

    Serial.printf("Modem: connecting to network: %s, is connected: %s\n", 
        fail ? "fail" : "success", 
        isConnected() ? "true" : "false");

    return !fail;
}

void ModemManager::disconnectFromNetwork() {
    Serial.println("Modem: disconnecting from network...");
#if TINY_GSM_USE_WIFI
    modem.networkDisconnect();
    Serial.println(F("Modem: WiFi disconnected"));
#endif
#if TINY_GSM_USE_GPRS
    modem.gprsDisconnect();
    // shut down modem pwr
    digitalWrite(BOARD_POWERON_PIN, LOW);
    Serial.println(F("Modem: GPRS disconnected"));
#endif
    Serial.println("Modem: Disconnecting from network: done");
}

/*
    Example: https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/blob/main/examples/ModemSleep/ModemSleep.ino
*/
bool ModemManager::enterSleepMode() {
    Serial.println("Modem: Entering modem power off...");

    bool poweroff = modem.poweroff();
    Serial.printf("Modem: Modem entered POWER OFF modem: %s\n", poweroff ? "SUCCESS" : "FAIL");

    delay(2000);

    // Serial.println("Modem: Checking modem response...");
    // while (modem.testAT(1000)) {
    //     delay(500);
    // }
    bool wentToSleep = !modem.testAT(1000);
    Serial.printf("Modem: Modem has enterd POWER OFF: %s\n", wentToSleep ? "TRUE" : "FALSE");

#ifdef BOARD_POWERON_PIN
    // Turn on DC boost to power off the modem
    digitalWrite(BOARD_POWERON_PIN, LOW);
#endif

#ifdef MODEM_RESET_PIN
    // Keep it low during the sleep period. If the module uses GPIO5 as reset, 
    // there will be a pulse when waking up from sleep that will cause the module to start directly.
    // https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/issues/85
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL);
    gpio_hold_en((gpio_num_t)MODEM_RESET_PIN);
    gpio_deep_sleep_hold_en();
#endif

    // false means entered sleep mode!
    return wentToSleep;
}

/*
    Example: https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/blob/main/examples/ModemSleep/ModemSleep.ino
*/
bool ModemManager::isModemOnline(const int retry) {
    int i = 0;
    while (i++ < retry) {
        bool connected = modem.testAT();
        if (connected) { return true; }
        delay(500);
    }
    return false;
}

TinyGsmClient& ModemManager::getGsmClient() {
    static TinyGsmClient client(modem);
    return client;
}

bool ModemManager::isConnected() {
#if TINY_GSM_USE_GPRS
    return modem.isGprsConnected();
#else
    return modem.isNetworkConnected();
#endif
}

bool ModemManager::waitForNetwork() { return modem.waitForNetwork(); }

int ModemManager::getSignalQuality() { return modem.getSignalQuality(); }

String ModemManager::getIpAddress() { return modem.getLocalIP(); }

String ModemManager::getModemName() { return modem.getModemName(); }

String ModemManager::getModemInfo() { return modem.getModemInfo(); }

String ModemManager::getNetworkMode() { return modem.getNetworkModes(); }

RegStatus ModemManager::getRegistrationStatus(const bool debug) {
    // Check network registration status and network signal status
    if (debug) Serial.println("Modem: Wait for the modem to register with the network.");
    int16_t sq;
    RegStatus status = REG_NO_RESULT;
    while (status == REG_NO_RESULT || status == REG_SEARCHING || status == REG_UNREGISTERED) {
        status = modem.getRegistrationStatus();
        switch (status) {
        case REG_UNREGISTERED:
        case REG_SEARCHING:
            sq = modem.getSignalQuality();
            if (debug) Serial.printf("Modem: [%lu] Signal Quality:%d\n", millis() / 1000, sq);
            delay(1000);
            break;
        case REG_DENIED:
            if (debug) Serial.println("Modem: Network registration was rejected, please check if the APN is correct");
            break;
        case REG_OK_HOME:
            Serial.println("Modem: Online registration successful");
            break;
        case REG_OK_ROAMING:
            if (debug) Serial.println("Modem: Network registration successful, currently in roaming mode");
            break;
        default:
            if (debug) Serial.printf("Modem: Registration Status:%d\n", status);
            delay(1000);
            break;
        }
    }
    if (debug) Serial.println();
    return status;
}

void ModemManager::printRegistrationStatus() {
    RegStatus status = getRegistrationStatus(true);
    Serial.printf("Modem: Registration Status: ");
    Serial.println(status);
}

bool ModemManager::setAccessPoint(const char* ssid, const char* pass) {
    WiFi.mode(WIFI_AP_STA);
    // Configure Wi-Fi network
    bool config = WiFi.softAPConfig(
        IpUtils::ipFromString(ap_local_ip), 
        IpUtils::ipFromString(ap_gateway_ip), 
        IpUtils::ipFromString(ap_subnet_mask));
    Serial.printf("Modem: Setting up the network: %s\n", config ? "success" : "fail");

    bool setup = WiFi.softAP(ssid, pass, 11, 0);
    Serial.printf("Modem: Access Point created: %s\n", setup ? "success" : "fail");
    if (!setup) { return false; }

    Serial.printf("Modem: Access Point IP address: %s\n", 
        getAccessPointIP().toString().c_str());
 
    return config;
}

bool ModemManager::forwardIncomingRequests() {
  WiFiClient wifiClient = WiFi.softAPgetStationNum(); 
  if (!wifiClient) {
    Serial.println("Modem: WiFi Client cannot be used");
    return false; 
  }

  static TinyGsmClient client(modem);
  if (!client.connected()) {
    Serial.println("Modem: TinyGsmClient not connected");
    return false; 
  }

  // Forward data from WiFi to Modem
  while (wifiClient.available()) {
    client.write(wifiClient.read());
    Serial.println("wifi received something");
  }

  // Forward data from Moddem to WiFi
  while (client.available()) {
    wifiClient.write(client.read());
    Serial.println("modem received");
  }
  return true;
}

IPAddress ModemManager::getAccessPointIP() {
    return WiFi.softAPIP();
}

uint8_t ModemManager::numOfAccessPointClients() {
    return WiFi.softAPgetStationNum();
}

std::vector<String> ModemManager::accessPointClientsConnected() {
    wifi_sta_list_t stationList;
    esp_wifi_ap_get_sta_list(&stationList);

    std::vector<String> clients;
    for (int i = 0; i < stationList.num; i++) {
        wifi_sta_info_t station = stationList.sta[i];
        String macStr = MacUtils::getMacAddressString(station.mac);
        clients.push_back(String(macStr));
    }
    return clients;
}


#endif
