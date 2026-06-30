#pragma once

#include <Arduino.h>
#include "utils/types.h"

class AutoTuner {
public:
  enum class Phase : uint8_t {
    IDLE,
    RELAY_HIGH,
    RELAY_LOW,
    COMPLETE
  };

  enum class Result : uint8_t {
    SUCCESS,
    TIMEOUT,
    CANCELLED,
    ERROR
  };

  struct Config {
    int heaterHigh = 400;
    int heaterLow = 0;
    float hysteresis = 0.2f;
    uint32_t minHalfPeriodMs = 3000;
    uint32_t timeoutMs = 600000;
    uint8_t requiredCycles = 4;
  };

  void begin(const Config& config, float setpoint);
  float update(float input);
  void start();
  void cancel();
  void reset();
  bool isRunning() const;
  bool isComplete() const;
  Phase getPhase() const;
  Result getResult() const;
  PIDTunings getTunings() const;
  float getProgress() const;

private:
  void computeTunings();
  void resetTracking();

  Config config_;
  float setpoint_ = 0.0f;
  Phase phase_ = Phase::IDLE;
  Result result_ = Result::ERROR;
  PIDTunings tunings_;

  int lastHeaterPwm_ = 0;
  uint32_t phaseStartTime_ = 0;
  uint32_t totalStartTime_ = 0;
  uint8_t cyclesCompleted_ = 0;
  float peakTemp_ = -1000.0f;
  float valleyTemp_ = 1000.0f;
  uint32_t lastPeakTime_ = 0;
  uint32_t lastValleyTime_ = 0;
  float lastError_ = 0.0f;
  bool firstCrossing_ = true;
  uint32_t peakPeriodSum_ = 0;
  uint8_t measurementCount_ = 0;
  float peakAmplitudeSum_ = -1000.0f;
  float valleyAmplitudeSum_ = 1000.0f;
  float valleyAmplitudeNext_ = 1000.0f;
};