#pragma once

#ifdef USE_ESPNOW

#include "ESP32_NOW.h"
#include "WiFi.h"

#include <vector>
#include <esp_mac.h>  // For the MAC2STR and MACSTR macros

#include "../Utils/NetworkUtils.h"
#include "../Utils/JsonUtils.h"

// 0: AP mode, 1: Station mode
#ifndef ESPNOW_WIFI_MODE_STATION
#define ESPNOW_WIFI_MODE_STATION 1
#endif

#if ESPNOW_WIFI_MODE_STATION          // ESP-NOW using WiFi Station mode
#define ESPNOW_WIFI_MODE WIFI_STA     // WiFi Mode
#define ESPNOW_WIFI_IF   WIFI_IF_STA  // WiFi Interface
#else                                 // ESP-NOW using WiFi AP mode
#define ESPNOW_WIFI_MODE WIFI_AP      // WiFi Mode
#define ESPNOW_WIFI_IF   WIFI_IF_AP   // WiFi Interface
#endif

// Channel to be used by the ESP-NOW protocol
#ifndef ESPNOW_WIFI_CHANNEL
#define ESPNOW_WIFI_CHANNEL 6
#endif

#define ESPNOW_BAUD 115200

#define ESPNOW_PEER_TYPE_DEFAULT "espnow_peer"

static const char *TAG = "ESP_NOW";

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
        JsonDocument doc = jsonFromDataReceived(data);

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


class EspNowPeer : public ESP_NOW_Peer {
public:
    // Constructors of the class
    EspNowPeer(const char *peer_type, const uint8_t *mac_addr, const uint8_t *lmk) 
    : ESP_NOW_Peer(mac_addr, ESPNOW_WIFI_CHANNEL, ESPNOW_WIFI_IF, lmk), peer_type(peer_type) { }

    EspNowPeer(const uint8_t *mac_addr, const uint8_t *lmk) 
    : ESP_NOW_Peer(mac_addr, ESPNOW_WIFI_CHANNEL, ESPNOW_WIFI_IF, lmk), peer_type(ESPNOW_PEER_TYPE_DEFAULT) { }

    // Destructor of the class
    ~EspNowPeer() {}

    // Function to register the peer
    bool addNewPeer() {
        if (!add()) {
            ESP_LOGE(TAG, "Failed to register peer: '%s'", peer_type);
            return false;
        }
        ESP_LOGI(TAG, "Peer added: '%s'", peer_type);
        return true;
    }

    // Function to send a message to all devices within the network
    bool sendMsg(const uint8_t *data, size_t len) {
        if (!send(data, len)) {
            ESP_LOGE(TAG, "Failed to send message to:'%s'", peer_type);
            return false;
        }
        ESP_LOGI(TAG, "Message sent to:'%s'", peer_type);
        return true;
    }

    void onReceive(const uint8_t *data, size_t len, bool broadcast) {
        ESP_LOGI(TAG, "Received a message from '%s' (%s): %s", peer_type, broadcast ? "broadcast" : "unicast", (char *)data);
        // EspNowResponse response = EspNowResponse::fromReceivedData(data);
        // response.print();
        if (on_receive_cb) {
            ESP_LOGD(TAG, "Receive callback triggered for peer: '%s'", peer_type);
            on_receive_cb(data, len, broadcast);
        }
    }

    void onSent(bool success) {
        ESP_LOGI(TAG, "Message transmission to peer '%s' %s", peer_type, success ? "successful" : "failed");
        if (on_sent_cb) {
            ESP_LOGD(TAG, "Send callback triggered for peer: '%s'", peer_type);
            on_sent_cb(success);
        }
    }

    void setOnReceiveCallback(void (*callback)(const uint8_t *, size_t, bool)) {
        on_receive_cb = callback;
    }

    void setOnSentCallback(void (*callback)(bool)) {
        on_sent_cb = callback;
    }

private:
    const char *peer_type;

    void (*on_receive_cb)(const uint8_t *, size_t, bool) = nullptr;
    void (*on_sent_cb)(bool) = nullptr;
};


class EspNow {
public:
    static bool begin() {
        // Initialize the Wi-Fi module
        ESP_LOGI(TAG, "ESP-NOW init has begun");
        WiFi.mode(ESPNOW_WIFI_MODE);
        WiFi.setChannel(ESPNOW_WIFI_CHANNEL);
        while (!WiFi.STA.started()) {
            delay(100);
        }

        ESP_LOGI(TAG, "ESP-NOW Wi-Fi parameters: Mode: %s, MAC Address: %s, Channel: %d", 
            ESPNOW_WIFI_MODE == WIFI_STA ? "STATION" : "AP", WiFi.macAddress().c_str(), ESPNOW_WIFI_CHANNEL);
            // Initialize the ESP-NOW protocol
        if (!ESP_NOW.begin()) {
            ESP_LOGE(TAG, "Failed to initialize ESP-NOW");
            return false;
        }
        ESP_LOGI(TAG, "ESP-NOW initialized");
        return true;
    }

    static void onNewPeerCb(void (*callback)(const esp_now_recv_info_t *info, const uint8_t *data, int len, void *arg), bool registerPeer) {
        ESP_NOW.onNewPeer(callback, NULL);
        ESP_LOGI(TAG, "ESP-NOW onNewPeer callback set");
        if (registerPeer) {
            registerNewPeer(info, data, len, arg);
        }
    }

    static bool removePeer(ESP_NOW_Peer &peer) {
        return ESP_NOW.removePeer(peer);
    }

private:
    static void registerNewPeer(const esp_now_recv_info_t *info, const uint8_t *data, int len, void *arg) {
        bool broadcast = memcmp(info->des_addr, ESP_NOW.BROADCAST_ADDR, 6) == 0;
        ESP_LOGI(TAG, "Unknown peer " MACSTR " sent a %s message\n", MAC2STR(info->src_addr), broadcast ? "broadcast" : "unicast");
        ESP_LOGI(TAG, "Registering the peer...");
    
        EspNowPeer new_peer(info->src_addr, NULL);
        if (!new_peer.addNewPeer()) {
            ESP_LOGE(TAG, "Failed to register the new peer " MACSTR "", MAC2STR(info->src_addr));
            return;
        }
        ESP_LOGI(TAG, "New peer registered: " MACSTR "", MAC2STR(info->src_addr));

    }
};

#endif
