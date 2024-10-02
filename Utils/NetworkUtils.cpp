#include "NetworkUtils.h"

#ifdef USE_NETWORK_UTILS

uint8_t* MacUtils::getDeviceMacAddress() {
    // init wifi
    WiFi.mode(WIFI_STA);

    // get MAC address
    static uint8_t baseMac[6];
    esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
    if (ret != ESP_OK) {
        Serial.println("getDeviceMacAddress(): Failed to read MAC address, returning empty...");
        // Return all zeros in case of failure
        memset(baseMac, 0, sizeof(baseMac));
    }
    return baseMac;
}

void MacUtils::printMacAddress(uint8_t* macAddress) {
    Serial.printf("MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
                macAddress[0], macAddress[1], macAddress[2],
                macAddress[3], macAddress[4], macAddress[5]);
}


String MacUtils::getMacAddressString(const uint8_t* macAddress) {
     // MAC address is 17 characters + null terminator
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
             macAddress[0], macAddress[1], macAddress[2],
             macAddress[3], macAddress[4], macAddress[5]);
    return String(macStr);
}

uint8_t* MacUtils::macStrToArray(const char* macAddress) {
    static uint8_t macArray[6];
    int values[6];

    if (sscanf(macAddress, "%x:%x:%x:%x:%x:%x", &values[0], &values[1], &values[2], &values[3], &values[4], &values[5]) == 6) {
        for (int i = 0; i < 6; ++i) {
            macArray[i] = (uint8_t)values[i];
        }
        return macArray;
    }

    // Handle the error if the MAC address string is not in the expected format
    Serial.printf("MacUtils: Invalid MAC address format: %s\n", macAddress);
    return NULL;
}

IPAddress IpUtils::ipFromString(const char* ip) {
    IPAddress addr;
    if (!addr.fromString(ip)) {
        Serial.println("Cannot convert string to IP address");
        return IPAddress();
    }
    return addr;
}

#endif
