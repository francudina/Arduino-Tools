#ifdef USE_VARIABLES

#include "Variables.h"

String DeviceVariable::setVariable(const char* pref, const char* var, const char* val) {
    Preferences p;
    p.begin(prefMetadata, false);
    String v = String(val);
    size_t s = p.putString(var, v);
    p.end();
    return s == 0 ? "" : v;
}

String DeviceVariable::getVariable(const char* pref, const char* var, const char* defaultVal) {
    Preferences p;
    p.begin(prefMetadata, true);
    String val = p.getString(var, String(defaultVal));
    p.end();
    return val;
}

uint32_t DeviceVariable::setVariableInt(const char* pref, const char* var, const uint32_t val) {
    Preferences p;
    p.begin(prefMetadata, false);
    p.putInt(var, val);
    p.end();
    return val;
}

uint32_t DeviceVariable::getVariableInt(const char* pref, const char* var, const uint32_t defaultVal) {
    Preferences p;
    p.begin(prefMetadata, true);
    uint32_t val = p.getInt(var, defaultVal);
    p.end();
    return val;
}

bool DeviceVariable::delVariable(const char* pref, const char* var) {
    Preferences p;
    p.begin(prefMetadata, false);
    bool r = p.remove(var);
    p.end();
    return r;
}

bool DeviceVariable::clear(const char* pref) {
    Preferences p;
    p.begin(prefMetadata, false);
    bool c = p.clear();
    p.end();
    return c;
}

String DeviceSoftware::getVersion() {
    Preferences pref;
    pref.begin(prefMetadata, true);
    String version = pref.getString(prefMetadata_softwareVersion, "v1.0.0");
    pref.end();
    return version;
}

String DeviceSoftware::getVersion(Preferences &pref) {
    return pref.getString(prefMetadata_softwareVersion, "v1.0.0");
}

String DeviceSoftware::setVersion() {
    Preferences pref;
    pref.begin(prefMetadata, false);

    String current = getVersion(pref);
    String next = versionFormat(device_swVersion_major, device_swVersion_minor, device_swVersion_patch);
    if (current != next) {
        pref.putString(prefMetadata_softwareVersion, next);
    }
    pref.end();
    Serial.printf("Versions: Device Software Version (previous: %s, current: %s)\n", current.c_str(), next.c_str());
    return next;
}

uint32_t DeviceSoftware::getBootNumber() {
    Preferences metadataPreference;
    metadataPreference.begin(prefMetadata, false);
    // booting info
    // number of bootups (can be used as count for deepsleeps)
    uint32_t bootCount = metadataPreference.getULong(prefMetadata_bootCount, 0);
    metadataPreference.end();
    return bootCount;
}

uint32_t DeviceSoftware::setBootNumber() {
    Preferences metadataPreference;
    metadataPreference.begin(prefMetadata, false);
    // booting info
    // number of bootups (can be used as count for deepsleeps)
    uint32_t bootCount = metadataPreference.getULong(prefMetadata_bootCount, 0);
    metadataPreference.putULong(prefMetadata_bootCount, ++bootCount);
    metadataPreference.end();
    return bootCount;
}

bool DeviceSoftware::resetBootNumber() {
    Preferences metadataPreference;
    metadataPreference.begin(prefMetadata, false);
    bool rem = metadataPreference.remove(prefMetadata_bootCount);
    metadataPreference.end();
    return rem;
}

String DeviceSoftware::versionFormat(const char *major, const char *minor, const char *patch) {
    return "v" + String(major) + "." + String(minor) + "." + String(patch);
}

#endif
