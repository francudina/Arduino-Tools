#pragma once

#ifdef USE_HTTP_CLIENT

#include <ArduinoJson.h>
#include <ArduinoHttpClient.h>

#include "../Utils/JsonUtils.h"

class HttpRequestHandler {
public:
    HttpRequestHandler(Client& client, const char* server, int port);
    int performGetRequest(const char* resource);
    int getResponseStatusCode();
    String getResponseBody();
    void stop();
    JsonDocument performGetRequestAndGetData(const char* resource, const size_t numOfKeys);

private:
    HttpClient httpClient;
};

#endif
