#pragma once

#include <esp_now.h>
#include <WiFi.h>

#include "NetworkUtils.h"
#include "JsonUtils.h"

typedef struct EspNowResponse {
    String topic;
    String data;
    int status;

    // Constructor to initialize to null state
    EspNowResponse() : topic(""), data(""), status(-1) {}

    // Static method to create a null EspNowResponse
    static EspNowResponse createNull() {
        return EspNowResponse();
    }

    // Method to check if this Response is null
    bool isNull() const {
        return topic.length() == 0 && data.length() == 0 && status == -1;
    }

    // Static method to deserialize from JSON
    static EspNowResponse fromReceivedData(const uint8_t *data) {
        DynamicJsonDocument doc = jsonFromDataReceived(50, data);

        EspNowResponse response;
        response.topic = doc["topic"].as<String>();
        response.data = doc["data"].as<String>();
        response.status = doc["status"];

        return response;
    }

    // Method to print the Response
    void print() const {
        Serial.println("ESP-NOW Message Received: ");
        Serial.print("\t> Topic: ");
        Serial.println(topic);
        Serial.print("\t> Data: ");
        Serial.println(data);
        Serial.print("\t> Status: ");
        Serial.println(status);
    }

} EspNowResponse;

class EspNowClient {
public:
    static bool init();
    static bool deinit();
    
    static void setOnReceiveCallback(void (*callback)(const uint8_t *, const uint8_t *, int));
    static void onReceiveDefaultCallback(const uint8_t *mac_addr, const uint8_t *data, int data_len);
    static void removeOnReceiveCallback();

    static bool sendMessage(const uint8_t *mac_addr, const uint8_t *data);

private:
    static void setOnSendCallback(const uint8_t *mac_addr, esp_now_send_status_t status);
};
