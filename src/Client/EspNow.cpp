// #ifdef USE_ESP_NOW

// #include "EspNow.h"

// bool EspNowClient::init(const int channel) {
//     ESP_LOGI(TAG, "WiFi Mode: %s", ESPNOW_WIFI_MODE == WIFI_AP ? "AP" : "Station");
//     WiFi.mode(ESPNOW_WIFI_MODE);

//     ESP_LOGI(TAG, "WiFi Channel: %d", channel);
//     WiFi.setChannel(channel, WIFI_SECOND_CHAN_NONE);

//     while (!(WiFi.STA.started() || WiFi.AP.started())) {
//         delay(100);
//     }

//     String macAddress = ESPNOW_WIFI_MODE == WIFI_IF_AP ? WiFi.softAPmacAddress() : WiFi.macAddress();
//     ESP_LOGI(TAG, "MAC Address: ", macAddress.c_str());

//     // Start the ESP-NOW communication
//     ESP_LOGI(TAG, "ESP-NOW communication starting...");
//     NowSerial.begin(ESPNOW_BAUD);
//     ESP_LOGI(TAG, "You can now send data to the peer device using the Serial Monitor.\n");
// }

// bool EspNowClient::deinit() {
//     bool unregister = esp_now_unregister_send_cb() == ESP_OK 
//                 && esp_now_unregister_recv_cb() == ESP_OK;
//     if (esp_now_deinit() != ESP_OK) {
//         Serial.println("EspNowClient: Failed to deinitialize ESP-NOW");
//     }
//     return unregister;
// }

// void EspNowClient::setOnReceiveCallback(void (*callback)(const uint8_t *, const uint8_t *, int)) {
//     esp_now_register_recv_cb(callback);
//     Serial.println("EspNowClient: Callback Set");
// }

// void EspNowClient::onReceiveDefaultCallback(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
//     // todo: define default callback for topic messages
//     EspNowResponse response = EspNowResponse::fromReceivedData(data);
//     response.print();
// }

// void EspNowClient::removeOnReceiveCallback() {
//     // when removing, set default callback, and prevent missing some messages!
//     esp_now_register_recv_cb(onReceiveDefaultCallback);
//     Serial.println("EspNowClient: Callback Set to Default");
// }

// void EspNowClient::setOnSendCallback(const uint8_t *mac_addr, esp_now_send_status_t status) {
// #ifdef DEVICE_LOGGING
//     Serial.printf("EspNowClient: Packet Send to '%s' Status: %s\n",
//         MacUtils::getMacAddressString(mac_addr).c_str(),
//         status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
// #endif
// }

// bool EspNowClient::sendMessage(const uint8_t *mac_addr, const uint8_t *data) {
//     esp_err_t result = esp_now_send(mac_addr, (uint8_t *) &data, sizeof(data));
//     return result == ESP_OK;
// }

// #endif
