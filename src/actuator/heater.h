#pragma once

#include <Arduino.h>
#include "config/pin_config.h"
#include "utils/types.h"

class Heater {
public:
  void begin();
  void setPWM(int value);
  void stop();
  int getCurrentPwm() const;

private:
  int currentPwm_ = 0;
};
