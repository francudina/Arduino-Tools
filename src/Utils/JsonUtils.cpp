#ifdef USE_JSON

#include "JsonUtils.h"

// Function to create an empty JsonDocument with defined capacity
JsonDocument createJsonDocument() {
    // Estimate the capacity required for the JSON document
    JsonDocument jd;
    return jd;
}

JsonDocument getJsonFromString(const String &jsonString) {
    JsonDocument doc = createJsonDocument();
    // Deserialize the JSON string into the JsonDocument
    DeserializationError error = deserializeJson(doc, jsonString);
    if (error) {
        Serial.print(F("deserializeJson(): failed: "));
        Serial.println(error.f_str());
    }
    return doc;
}

String getStringFromJson(const JsonDocument &doc) {
    // Create a string to hold the serialized JSON
    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
}

bool isJsonEmpty(const JsonDocument doc) { return doc.isNull(); }

JsonDocument jsonFromDataReceived(const uint8_t *data) {
    String jsonString = String((const char*) data);
    return getJsonFromString(jsonString);
}

#endif
