#pragma once

#ifdef USE_JSON_UTILS

#include <ArduinoJson.h>

DynamicJsonDocument createJsonDocument(const size_t numOfKeys);
DynamicJsonDocument getJsonFromString(const String &jsonString, const size_t numOfKeys);
String getStringFromJson(const DynamicJsonDocument &doc);
bool isJsonEmpty(const DynamicJsonDocument doc);

DynamicJsonDocument jsonFromDataReceived(const size_t numOfKeys, const uint8_t *data);

#endif
