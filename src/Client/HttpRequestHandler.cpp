#ifdef USE_HTTP_CLIENT

#include "HttpRequestHandler.h"

HttpRequestHandler::HttpRequestHandler(Client& client, const char* server, int port)
    : httpClient(client, server, port) {}

int HttpRequestHandler::performGetRequest(const char* resource) {
    return httpClient.get(resource);
}

int HttpRequestHandler::getResponseStatusCode() {
    return httpClient.responseStatusCode();
}

String HttpRequestHandler::getResponseBody() {
    return httpClient.responseBody();
}

void HttpRequestHandler::stop() {
    httpClient.stop();
}

JsonDocument HttpRequestHandler::performGetRequestAndGetData(const char* resource) {
    int httpCode = performGetRequest(resource);
    int statusCode = getResponseStatusCode();
    
    JsonDocument doc;
    if (httpCode != 0) {
        Serial.printf("HttpRequestHandler: Request failed with code: %d, response: %d\n", httpCode, statusCode);
        return doc;
    }

    if (statusCode != 200) {
        Serial.printf("HttpRequestHandler: Failed to fetch data! Status code: %d\n", statusCode);
        return doc;
    }

    String response = getResponseBody();
    doc = getJsonFromString(response);
    if (isJsonEmpty(doc)) { 
        Serial.printf("HttpRequestHandler: resulted with empty JSON data");
    }

    return doc;
}

#endif
