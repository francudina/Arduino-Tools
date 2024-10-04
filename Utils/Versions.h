#pragma once

#include <Preferences.h>
#include <cstring>

#include "../../src/config.h"

class DeviceSoftware {
public:
    static String getVersion();
    static String getVersion(Preferences &pref);

    static String setVersion();

    static uint32_t getBootNumber();
    static uint32_t setBootNumber();

private:
    static String versionFormat(const char *major, const char *minor, const char *patch);
};
