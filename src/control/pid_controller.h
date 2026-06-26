#pragma once

#include <cstdint>
#include "utils/types.h"
#include "config/system_config.h"

class PIDController {
public:
  void begin(const PIDTunings& t, float setpoint);
  
  float compute(float input);
  
  void setTunings(const PIDTunings& t);
  void setSetpoint(float sp);
  void setOutputLimits(float min, float max);
  void reset();
  
  PIDTunings getTunings() const;
  float getSetpoint() const;
  float getOutput() const;
  float getError() const;

private:
  PIDTunings tunings_;
  float setpoint_ = 0.0f;
  
  float integral_ = 0.0f;
  float prevError_ = 0.0f;
  float filteredDerivative_ = 0.0f;
  float output_ = 0.0f;
  float error_ = 0.0f;
  
  float outMin_ = Config::OUTPUT_MIN;
  float outMax_ = Config::OUTPUT_MAX;
  
  uint32_t lastComputeTime_ = 0;
  bool firstRun_ = true;
  
  static constexpr float ALPHA = Config::FILTER_COEFFICIENT;
  static constexpr float INV_ALPHA = 1.0f - ALPHA;
};
