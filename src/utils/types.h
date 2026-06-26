#pragma once

#include <cstdint>

enum class SensorStatus : uint8_t {
  OK,
  DISCONNECTED,
  ERROR,
  READING
};

enum class SystemState : uint8_t {
  INIT,
  NORMAL,
  SENSOR_FAULT,
  OVERTEMP,
  OFFLINE
};

struct PIDTunings {
  float kp = 0.0f;
  float ki = 0.0f;
  float kd = 0.0f;
};

struct SensorData {
  float temperature = 0.0f;
  SensorStatus status = SensorStatus::ERROR;
  uint32_t lastReadTime = 0;
};

struct ControlOutput {
  float pidResult = 0.0f;
  float error = 0.0f;
  int heaterPwm = 0;
  int fanPwm = 0;
};

struct SystemData {
  SensorData sensor;
  ControlOutput control;
  PIDTunings pid;
  float setpoint = 37.0f;
  int wifiRssi = 0;
  uint32_t freeHeap = 0;
  uint32_t uptime = 0;
  SystemState state = SystemState::INIT;
  bool alarm = false;
};
