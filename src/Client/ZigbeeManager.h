#pragma once

#ifdef USE_ZIGBEE

#ifdef ZIGBEE_GATEWAY
#ifndef ZIGBEE_MODE_ZCZR
#define ZIGBEE_MODE_ZCZR
#endif
#endif

#include <Zigbee.h>

// Zigbee Config
#ifndef ZIGBEE_CONFIG
#define ZIGBEE_ENDPOINT_NUMBER 1
#endif

#ifndef ZIGBEE_GATEWAY_CONFIG
#define GATEWAY_RCP_UART_PORT UART_NUM_1
#define GATEWAY_RCP_RX_PIN 4
#define GATEWAY_RCP_TX_PIN 5

#define GATEWAY_NETWORK_OPEN_TIME 180
#define GATEWAY_NERWORK_ALLOW_MULTIPLE_BINDING false

#define GATEWAY_MANUFACTURER "Espressif"
#define GATEWAY_MODEL "ZigbeeGateway_Custom"

#endif

class ZigbeeClient {
public:

    ZigbeeClient();

#ifdef ZIGBEE_GATEWAY
    bool gatewayStart(zigbee_role_t deviceRole);
#endif

    bool isConnected();
    void factoryReset();
    void openNetwork(uint8_t time);

private:
#ifdef ZIGBEE_GATEWAY
    ZigbeeGateway zbGateway;
#endif
};

#endif
