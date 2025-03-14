#ifdef USE_ZIGBEE

#ifdef ZIGBEE_GATEWAY
bool ZigbeeClient::gatewayStart(zigbee_role_t deviceRole) {
  zbGateway = ZigbeeGateway(ZIGBEE_ENDPOINT_NUMBER);
  zbGateway.setManufacturerAndModel(GATEWAY_MANUFACTURER, GATEWAY_MODEL);
  zbGateway.allowMultipleBinding(GATEWAY_NERWORK_ALLOW_MULTIPLE_BINDING);

  Zigbee.addEndpoint(&zbGateway);
  Zigbee.setRebootOpenNetwork(GATEWAY_NETWORK_OPEN_TIME);

  esp_zb_radio_config_t radio_config = ZIGBEE_DEFAULT_UART_RCP_RADIO_CONFIG();
  radio_config.radio_uart_config.port = GATEWAY_RCP_UART_PORT;
  radio_config.radio_uart_config.rx_pin = (gpio_num_t)GATEWAY_RCP_RX_PIN;
  radio_config.radio_uart_config.tx_pin = (gpio_num_t)GATEWAY_RCP_TX_PIN;
  Zigbee.setRadioConfig(radio_config);
  
  // ZIGBEE_COORDINATOR or ZIGBEE_ROUTER
  if (!Zigbee.begin(deviceRole)) ESP.restart();
}

#endif

bool ZigbeeClient::isConnected() {
    return Zigbee.connected();
}

bool ZigbeeClient::factoryReset() {
    return Zigbee.factoryReset();
}

void ZigbeeClient::openNetwork(uint8_t time) {
    return Zigbee.openNetwork(time);
}

#endif
