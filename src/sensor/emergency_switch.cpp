#include "sensor/emergency_switch.h"

void EmergencySwitch::begin() {
  pinMode(Pin::EMERGENCY_SWITCH, INPUT_PULLUP);
  pinMode(Pin::NORMAL_INDICATOR, INPUT_PULLUP);
  emergencyActive_ = digitalRead(Pin::EMERGENCY_SWITCH) == LOW;
  normalIndicator_ = digitalRead(Pin::NORMAL_INDICATOR) == LOW;
}

void EmergencySwitch::update() {
  bool rawEmergency = digitalRead(Pin::EMERGENCY_SWITCH) == LOW;
  bool rawNormal = digitalRead(Pin::NORMAL_INDICATOR) == LOW;

  if (rawEmergency != lastRawState_) {
    lastChangeTime_ = millis();
    lastRawState_ = rawEmergency;
  }

  if (millis() - lastChangeTime_ >= DEBOUNCE_MS) {
    emergencyActive_ = rawEmergency;
    normalIndicator_ = rawNormal;
  }
}

bool EmergencySwitch::isEmergency() const {
  return emergencyActive_;
}

bool EmergencySwitch::isNormalIndicatorActive() const {
  return normalIndicator_;
}
