#pragma once

#include <Arduino.h>
#include "config/pin_config.h"

class EmergencySwitch {
public:
  void begin();
  bool isEmergency() const;
  bool isNormalIndicatorActive() const;
  void update();

private:
  bool emergencyActive_ = false;
  bool normalIndicator_ = false;
  static constexpr uint32_t DEBOUNCE_MS = 50;
  uint32_t lastChangeTime_ = 0;
  bool lastRawState_ = false;
};
