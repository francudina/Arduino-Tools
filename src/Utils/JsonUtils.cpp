#ifdef USE_JSON

#include "JsonUtils.h"

// Function to create an empty DynamicJsonDocument with defined capacity
DynamicJsonDocument createJsonDocument(const size_t numOfKeys) {
    // Estimate the capacity required for the JSON document
    const size_t capacity = JSON_OBJECT_SIZE(numOfKeys);
    return DynamicJsonDocument(capacity);
}

DynamicJsonDocument getJsonFromString(const String &jsonString, const size_t numOfKeys) {
    // Estimate the capacity required for the JSON document
    const size_t capacity = JSON_OBJECT_SIZE(numOfKeys);
    DynamicJsonDocument doc(capacity);

    // Deserialize the JSON string into the DynamicJsonDocument
    DeserializationError error = deserializeJson(doc, jsonString);
    if (error) {
        Serial.print(F("deserializeJson(): failed: "));
        Serial.println(error.f_str());
        // Return a document with zero capacity to indicate failure
        return DynamicJsonDocument(0);
    }

    return doc;
}

String getStringFromJson(const DynamicJsonDocument &doc) {
    // Create a string to hold the serialized JSON
    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
}

bool isJsonEmpty(const DynamicJsonDocument doc) { return doc.capacity() == 0; }

DynamicJsonDocument jsonFromDataReceived(const size_t numOfKeys, const uint8_t *data) {
    String jsonString = String((const char*) data);
    return getJsonFromString(jsonString, numOfKeys);
}

#endif
