#ifdef USE_VARIABLES

#include "Variables.h"

String DeviceVariable::setVariable(const char* pref, const char* var, const char* val) {
    Preferences p;
    p.begin(prefMetadata, false);
    String v = String(val);
    p.putString(var, v);
    p.end();
    return v;
}

String DeviceVariable::getVariable(const char* pref, const char* var, const char* defaultVal) {
    Preferences p;
    p.begin(prefMetadata, true);
    String val = p.getString(var, String(defaultVal));
    p.end();
    return val;
}

void DeviceVariable::delVariable(const char* pref, const char* var) {
    Preferences p;
    p.begin(prefMetadata, false);
    p.remove(var);
    p.end();
}

void DeviceVariable::clear(const char* pref) {
    Preferences p;
    p.begin(prefMetadata, false);
    p.clear();
    p.end();
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

String DeviceSoftware::versionFormat(const char *major, const char *minor, const char *patch) {
    return "v" + String(major) + "." + String(minor) + "." + String(patch);
}

#endif
