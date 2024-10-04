#include "ModemManager.h"

ModemManager::ModemManager() : modem(SerialAT) { }

bool ModemManager::modemInit() {
    Serial.println("Modem: modem init...");
    SerialAT.begin(MODEM_BAUD, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);

    bool modemBoot;
    bool wakeUpCause = esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER;
    if (wakeUpCause) {
        modemBoot = wakeupFromSleepMode();
    } else {
        pinMode(MODEM_PWR_PIN, OUTPUT);
        digitalWrite(MODEM_PWR_PIN, HIGH);
        delay(500);
        modemBoot = isModemOnline();
    }
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
        bool connected = connectToNetwork(wifiSSID, wifiPass, apn, gprsUser, gprsPass);
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
    digitalWrite(MODEM_PWR_PIN, LOW);
    Serial.println(F("Modem: GPRS disconnected"));
#endif
    Serial.println("Modem: disconnecting from network: done");
}

/*
    Example: https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/blob/main/examples/ModemSleep/ModemSleep.ino
*/
bool ModemManager::enterSleepMode() {
    // Pull up DTR to put the modem into sleep
    pinMode(MODEM_DTR_PIN, OUTPUT);
    digitalWrite(MODEM_DTR_PIN, HIGH);
    // Set DTR to keep at high level, if not set, DTR will be invalid after ESP32 goes to sleep.
    gpio_hold_en((gpio_num_t) MODEM_DTR_PIN);
    gpio_deep_sleep_hold_en();
    delay(500);

    bool wentToSleep = modem.sleepEnable(true);
    Serial.printf("Modem: Modem went to sleep: %s\n", wentToSleep ? "success" : "failed");
    
    gpio_hold_en((gpio_num_t) MODEM_PWR_PIN);
    gpio_deep_sleep_hold_en();

    // false means entered sleep mode!
    return !isModemOnline();
}

/*
    Example: https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/blob/main/examples/ModemSleep/ModemSleep.ino
*/
bool ModemManager::wakeupFromSleepMode() {
     // Need to cancel GPIO hold if wake from sleep
    gpio_hold_dis((gpio_num_t) MODEM_DTR_PIN);
    // Need to cancel GPIO hold if wake from sleep
    gpio_hold_dis((gpio_num_t) MODEM_PWR_PIN);

    // Turn on DC boost to power on the modem
    pinMode(MODEM_PWR_PIN, OUTPUT);
    digitalWrite(MODEM_PWR_PIN, HIGH);
    // Pull down DTR to wake up MODEM
    pinMode(MODEM_DTR_PIN, OUTPUT);
    digitalWrite(MODEM_DTR_PIN, LOW);
    delay(200);

    bool wokenUp = modem.sleepEnable(false);
    Serial.printf("Modem: Modem has woken up from sleep: %s\n", wokenUp ? "success" : "failed");

    // Delay sometime ...
    delay(500);
    // true means exited sleep mode!
    return isModemOnline();
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
