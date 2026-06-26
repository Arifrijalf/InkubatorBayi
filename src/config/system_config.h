#pragma once

#include <cstdint>

namespace Config {
  constexpr uint32_t PID_SAMPLE_TIME_MS = 500;
  constexpr uint32_t SENSOR_READ_INTERVAL_MS = 1000;
  constexpr uint32_t WS_BROADCAST_INTERVAL_MS = 500;
  constexpr uint32_t SENSOR_RETRY_INTERVAL_MS = 5000;

  constexpr float TEMP_MIN = 20.0f;
  constexpr float TEMP_MAX = 45.0f;
  constexpr float OUTPUT_MIN = 0.0f;
  constexpr float OUTPUT_MAX = 1023.0f;

  constexpr float TEMP_FAULT_LOW = 15.0f;
  constexpr float TEMP_FAULT_HIGH = 50.0f;

  constexpr float FILTER_COEFFICIENT = 0.3f;
  constexpr uint32_t INTEGRAL_WINDUP_LIMIT = 1000;

  constexpr float ALARM_OVERTEMP_OFFSET = 5.0f;

  namespace Default {
    constexpr float SETPOINT = 37.0f;
    constexpr float KP = 2.0f;
    constexpr float KI = 0.5f;
    constexpr float KD = 1.0f;
  }

  constexpr const char* AP_SSID = "InkubatorAP";
  constexpr const char* AP_PASS = "12345678";
  constexpr uint8_t MAX_WIFI_RETRY = 20;
  constexpr uint32_t WIFI_RETRY_DELAY_MS = 500;

  constexpr uint16_t WEB_SERVER_PORT = 80;
  constexpr uint16_t WS_PATH_MAX = 16;
}
