#include "storage/preferences_store.h"

void PreferencesStore::begin() {
  prefs_.begin(NS_, false);
}

PIDTunings PreferencesStore::loadPIDTunings() {
  PIDTunings t;
  t.kp = prefs_.getFloat(KEY_KP, Config::Default::KP);
  t.ki = prefs_.getFloat(KEY_KI, Config::Default::KI);
  t.kd = prefs_.getFloat(KEY_KD, Config::Default::KD);
  return t;
}

void PreferencesStore::savePIDTunings(const PIDTunings& t) {
  prefs_.putFloat(KEY_KP, t.kp);
  prefs_.putFloat(KEY_KI, t.ki);
  prefs_.putFloat(KEY_KD, t.kd);
}

float PreferencesStore::loadSetpoint() {
  return prefs_.getFloat(KEY_SP, Config::Default::SETPOINT);
}

void PreferencesStore::saveSetpoint(float sp) {
  prefs_.putFloat(KEY_SP, sp);
}

void PreferencesStore::loadWiFiCredentials(String& ssid, String& pass) {
  ssid = prefs_.getString(KEY_SSID, Config::AP_SSID);
  pass = prefs_.getString(KEY_PASS, Config::AP_PASS);
}

void PreferencesStore::saveWiFiCredentials(const String& ssid, const String& pass) {
  prefs_.putString(KEY_SSID, ssid);
  prefs_.putString(KEY_PASS, pass);
}
