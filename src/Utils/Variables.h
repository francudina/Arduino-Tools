#pragma once

#ifdef USE_VARIABLES

#include <Preferences.h>
#include <cstring>

// Preferences config
// - general info
#ifndef PREF_GENERAL_INFO
#define prefMetadata                        "metadata"
#define prefMetadata_bootCount              "boot_count"
#define prefMetadata_loopCount              "loop_count"
#define prefMetadata_macAddress             "mac_address"
#define prefMetadata_softwareVersion        "sw_version"
#endif

// - software
#ifndef DEVICE_DETAILS
const char device_swVersion_major[] = "1";
const char device_swVersion_minor[] = "0";
const char device_swVersion_patch[] = "0";
#endif

class DeviceVariable {
    public:    
        static String setVariable(const char* pref, const char* var, const char* val);
        static String getVariable(const char* pref, const char* var, const char* defaultVal = "");

        static uint32_t setVariableInt(const char* pref, const char* var, const uint32_t val);
        static uint32_t getVariableInt(const char* pref, const char* var, const uint32_t defaultVal = -1);

        static bool setVariableBool(const char* pref, const char* var, const bool val);
        static bool getVariableBool(const char* pref, const char* var, const bool defaultVal = false);

        static bool delVariable(const char* pref, const char* var);
        static bool clear(const char* pref);
    };

class DeviceSoftware {
public:
    static String getVersion();
    static String getVersion(Preferences &pref);

    static String setVersion();

    static uint32_t getBootNumber();
    static uint32_t setBootNumber();
    static bool resetBootNumber();

private:
    static String versionFormat(const char *major, const char *minor, const char *patch);
};

#endif
