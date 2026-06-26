#pragma once

#include <Preferences.h>
#include "config/system_config.h"
#include "utils/types.h"

class PreferencesStore {
public:
  void begin();
  
  PIDTunings loadPIDTunings();
  void savePIDTunings(const PIDTunings& t);
  
  float loadSetpoint();
  void saveSetpoint(float sp);
  
  void loadWiFiCredentials(String& ssid, String& pass);
  void saveWiFiCredentials(const String& ssid, const String& pass);

private:
  Preferences prefs_;
  
  static constexpr const char* NS_ = "inkub";
  static constexpr const char* KEY_KP = "kp";
  static constexpr const char* KEY_KI = "ki";
  static constexpr const char* KEY_KD = "kd";
  static constexpr const char* KEY_SP = "setpoint";
  static constexpr const char* KEY_SSID = "ssid";
  static constexpr const char* KEY_PASS = "pass";
};
