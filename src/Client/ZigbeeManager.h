#pragma once

#ifdef USE_ZIGBEE

#include <Zigbee.h>

// Zigbee Config
#ifndef ZIGBEE_CONFIG
#define ZIGBEE_ENDPOINT_NUMBER 1
#endif

#ifdef ZIGBEE_GATEWAY_CONFIG
#define GATEWAY_RCP_UART_PORT UART_NUM_1
#define GATEWAY_RCP_RX_PIN 4
#define GATEWAY_RCP_TX_PIN 5

#define GATEWAY_NETWORK_OPEN_TIME 180
#define GATEWAY_NERWORK_ALLOW_MULTIPLE_BINDING false

#define GATEWAY_MANUFACTURER "Espressif"
#define GATEWAY_MODEL "ZigbeeGateway_Custom"

#ifndef ZIGBEE_MODE_ZCZR
#error "Zigbee coordinator mode is not selected in Tools->Zigbee mode"
#endif

#endif

class ZigbeeClient {
public:

#ifdef ZIGBEE_GATEWAY
    bool gatewayStart(zigbee_role_t deviceRole);
#endif

    bool isConnected();
    bool factoryReset();
    void openNetwork(uint8_t time);

private:
    ZigbeeGateway zbGateway;
};

#endif
