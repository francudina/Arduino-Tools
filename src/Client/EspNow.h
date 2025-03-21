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

typedef struct EspNowMsg {
    String topic;
    String data;
    int status;

    // Constructor to initialize to null state
    EspNowMsg() : topic(""), data(""), status(-1) {}

    // Static method to create a null EspNowMsg
    static EspNowMsg createNull() {
        return EspNowMsg();
    }

    // Method to check if this Response is null
    bool isNull() const {
        return topic.length() == 0 && data.length() == 0 && status == -1;
    }

    // Static method to deserialize from JSON
    static EspNowMsg fromReceivedData(const String data) {
        JsonDocument doc = getJsonFromString(data);
        EspNowMsg m;
        if (!doc.isNull()) {
            m.topic = doc["topic"].as<String>();
            m.data = doc["data"].as<String>();
            m.status = doc["status"];
        }
        return m;
    }

    static EspNowMsg fromReceivedRawData(const uint8_t *data) {
        if (sizeof(data) == 0) {
            EspNowMsg m;
            return m;
        };
        String d = String((const char*)data);
        return fromReceivedData(d);
    }

    String toString() {
        JsonDocument m;
        m["topic"] = topic;
        m["data"] = data;
        m["status"] = status;
        return getStringFromJson(m);
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

} EspNowMsg;


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

    bool waitPeerToJoin(const int wait) {
        long s = millis(); bool joined = false;
        while (!joined && (millis() - s) < wait)  {
          if (addNewPeer()) joined = true;
          delay(250);
        }
        return joined;
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

    bool waitMsgToBeSent(const String message, const int wait) {
        long s = millis(); int i = 0; bool sent = false;
        while (!message.isEmpty() && !sent && (millis() - s) < wait) {
            if (sendMsg((const uint8_t *)message.c_str(), message.length())) {
                sent = true;
                break;
            }
            delay(1000);
        }
        return sent;
    }

    void onReceive(const uint8_t *data, size_t len, bool broadcast) {
        ESP_LOGD(TAG, "Received a message from '%s' (%s): %s", peer_type, broadcast ? "broadcast" : "unicast", (char *)data);
        if (on_receive_cb) {
            ESP_LOGV(TAG, "Receive callback triggered for peer: '%s'", peer_type);
            on_receive_cb(data, len, broadcast);
        }
    }

    void onSent(bool success) {
        ESP_LOGD(TAG, "Message transmission to peer '%s' %s", peer_type, success ? "successful" : "failed");
        if (on_sent_cb) {
            ESP_LOGV(TAG, "Send callback triggered for peer: '%s'", peer_type);
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
    static bool begin(const uint8_t *pmk) {
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
        if (!ESP_NOW.begin(pmk)) {
            ESP_LOGE(TAG, "Failed to initialize ESP-NOW");
            return false;
        }
        ESP_LOGI(TAG, "ESP-NOW initialized");
        return true;
    }

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

    static void onNewPeerCb(void (*callback)(const esp_now_recv_info_t *info, const uint8_t *data, int len, void *arg)) {
        ESP_NOW.onNewPeer(callback, NULL);
        ESP_LOGI(TAG, "ESP-NOW onNewPeer callback set");
    }

    static bool removePeer(ESP_NOW_Peer &peer) {
        return ESP_NOW.removePeer(peer);
    }

    static bool end() {
        return ESP_NOW.end();
    }
};

#endif
