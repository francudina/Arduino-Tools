#pragma once

#ifdef USE_JSON

#include <ArduinoJson.h>

JsonDocument createJsonDocument();
JsonDocument getJsonFromString(const String &jsonString);
String getStringFromJson(const JsonDocument &doc);
bool isJsonEmpty(const JsonDocument doc);

JsonDocument jsonFromDataReceived(const uint8_t *data);

#endif
