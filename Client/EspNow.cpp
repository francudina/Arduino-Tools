#include "EspNow.h"

#ifdef USE_ESPNOW

bool EspNowClient::init() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    if (esp_now_init() != ESP_OK) {
        Serial.println("EspNowClient: Error initializing ESP-NOW");
        return false;
    }
    esp_now_register_send_cb(setOnSendCallback);
    esp_now_register_recv_cb(onReceiveDefaultCallback);
    Serial.println("EspNowClient: ESP-NOW Initialized, Send & Default Receive Callback set");
    return true;
}

bool EspNowClient::deinit() {
    bool unregister = esp_now_unregister_send_cb() == ESP_OK 
                && esp_now_unregister_recv_cb() == ESP_OK;
    if (esp_now_deinit() != ESP_OK) {
        Serial.println("EspNowClient: Failed to deinitialize ESP-NOW");
    }
    return unregister;
}

void EspNowClient::setOnReceiveCallback(void (*callback)(const uint8_t *, const uint8_t *, int)) {
    esp_now_register_recv_cb(callback);
    Serial.println("EspNowClient: Callback Set");
}

void EspNowClient::onReceiveDefaultCallback(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
    // todo: define default callback for topic messages
    EspNowResponse response = EspNowResponse::fromReceivedData(data);
    response.print();
}

void EspNowClient::removeOnReceiveCallback() {
    // when removing, set default callback, and prevent missing some messages!
    esp_now_register_recv_cb(onReceiveDefaultCallback);
    Serial.println("EspNowClient: Callback Set to Default");
}

void EspNowClient::setOnSendCallback(const uint8_t *mac_addr, esp_now_send_status_t status) {
#ifdef DEVICE_LOGGING
    Serial.printf("EspNowClient: Packet Send to '%s' Status: %s\n",
        MacUtils::getMacAddressString(mac_addr).c_str(),
        status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
#endif
}

bool EspNowClient::sendMessage(const uint8_t *mac_addr, const uint8_t *data) {
    esp_err_t result = esp_now_send(mac_addr, (uint8_t *) &data, sizeof(data));
    return result == ESP_OK;
}

#endif
