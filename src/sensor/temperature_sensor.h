#pragma once

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "config/pin_config.h"
#include "config/system_config.h"
#include "utils/types.h"

class TemperatureSensor {
public:
  void begin();
  void update();
  float getTemperature() const;
  SensorStatus getStatus() const;
  bool isConnected() const;

private:
  OneWire oneWire_;
  DallasTemperature sensors_;
  SensorData data_;
  uint32_t lastRequestTime_ = 0;
  bool waitingForConversion_ = false;
  float lastValidTemp_ = 0.0f;

  static constexpr int INVALID_RESULT = -127;
  static constexpr int DEVICE_INDEX = 0;
  
  void requestTemperature();
  bool readTemperature();
};
