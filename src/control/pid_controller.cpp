#include <Arduino.h>
#include "control/pid_controller.h"

void PIDController::begin(const PIDTunings& t, float setpoint) {
  tunings_ = t;
  setpoint_ = setpoint;
  reset();
}

float PIDController::compute(float input) {
  uint32_t now = millis();
  uint32_t elapsed = now - lastComputeTime_;
  
  if (elapsed < Config::PID_SAMPLE_TIME_MS) {
    return output_;
  }
  
  float dt = static_cast<float>(elapsed) / 1000.0f;
  
  error_ = setpoint_ - input;
  
  integral_ += tunings_.ki * error_ * dt;
  if (integral_ > Config::INTEGRAL_WINDUP_LIMIT) {
    integral_ = Config::INTEGRAL_WINDUP_LIMIT;
  } else if (integral_ < -Config::INTEGRAL_WINDUP_LIMIT) {
    integral_ = -Config::INTEGRAL_WINDUP_LIMIT;
  }
  
  float rawDerivative = (error_ - prevError_) / dt;
  filteredDerivative_ = ALPHA * rawDerivative + INV_ALPHA * filteredDerivative_;
  
  float pTerm = tunings_.kp * error_;
  float iTerm = integral_;
  float dTerm = tunings_.kd * filteredDerivative_;
  
  output_ = pTerm + iTerm + dTerm;
  output_ = constrain(output_, outMin_, outMax_);
  
  prevError_ = error_;
  lastComputeTime_ = now;
  
  return output_;
}

void PIDController::setTunings(const PIDTunings& t) {
  tunings_ = t;
}

void PIDController::setSetpoint(float sp) {
  setpoint_ = sp;
}

void PIDController::setOutputLimits(float min, float max) {
  outMin_ = min;
  outMax_ = max;
}

void PIDController::reset() {
  integral_ = 0.0f;
  prevError_ = 0.0f;
  filteredDerivative_ = 0.0f;
  output_ = 0.0f;
  error_ = 0.0f;
  firstRun_ = true;
  lastComputeTime_ = millis();
}

PIDTunings PIDController::getTunings() const {
  return tunings_;
}

float PIDController::getSetpoint() const {
  return setpoint_;
}

float PIDController::getOutput() const {
  return output_;
}

float PIDController::getError() const {
  return error_;
}
