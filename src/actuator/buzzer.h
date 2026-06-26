#pragma once

#include <Arduino.h>
#include "config/pin_config.h"

class Buzzer {
public:
  void begin();
  void on();
  void off();
  void update(bool alarmActive);
  bool isOn() const;

private:
  bool active_ = false;
};
