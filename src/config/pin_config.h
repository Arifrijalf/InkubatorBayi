#pragma once

#include <cstdint>

namespace Pin {
  constexpr uint8_t ONE_WIRE_BUS = 19;
  constexpr uint8_t HEATER_PWM = 21;
  constexpr uint8_t FAN_PWM = 22;
  constexpr uint8_t BUZZER = 23;
  constexpr uint8_t EMERGENCY_SWITCH = 27;
  constexpr uint8_t NORMAL_INDICATOR = 26;
}

namespace PWM {
  namespace Heater {
    constexpr uint8_t CHANNEL = 0;
    constexpr uint8_t TIMER = 0;
    constexpr uint32_t FREQ_HZ = 5000;
    constexpr uint8_t RESOLUTION_BITS = 10;
    constexpr uint16_t MAX_VALUE = 1023;
  }
  namespace Fan {
    constexpr uint8_t CHANNEL = 1;
    constexpr uint8_t TIMER = 1;
    constexpr uint32_t FREQ_HZ = 25000;
    constexpr uint8_t RESOLUTION_BITS = 10;
    constexpr uint16_t MAX_VALUE = 1023;
    constexpr uint16_t MIN_VALUE = 200;
  }
}
