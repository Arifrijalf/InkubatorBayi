#include "control/auto_tuner.h"
#include "config/system_config.h"

void AutoTuner::begin(const Config& config, float setpoint) {
  config_ = config;
  setpoint_ = setpoint;
  reset();
}

float AutoTuner::update(float input) {
  if (phase_ != Phase::RELAY_HIGH && phase_ != Phase::RELAY_LOW) {
    return 0.0f;
  }

  uint32_t now = millis();
  if (now - totalStartTime_ > config_.timeoutMs) {
    result_ = Result::TIMEOUT;
    phase_ = Phase::COMPLETE;
    return 0.0f;
  }

  bool heaterOn = (phase_ == Phase::RELAY_HIGH);
  float error = setpoint_ - input;

  if (heaterOn) {
    if (input > peakTemp_) peakTemp_ = input;
  } else {
    if (input < valleyTemp_) valleyTemp_ = input;
  }

  if (heaterOn && error < -config_.hysteresis) {
    // Peak detected: temperature crossing setpoint + hysteresis going down
    if (!firstCrossing_) {
      cyclesCompleted_++;
    }

    if (cyclesCompleted_ > 2 && lastPeakTime_ > 0) {
      peakPeriodSum_ += (now - lastPeakTime_);
      measurementCount_++;
      if (peakTemp_ > peakAmplitudeSum_) peakAmplitudeSum_ = peakTemp_;
    }
    lastPeakTime_ = now;
    valleyTemp_ = input;
    phase_ = Phase::RELAY_LOW;
    firstCrossing_ = false;

  } else if (!heaterOn && error > config_.hysteresis) {
    // Valley detected: temperature crossing setpoint - hysteresis going up
    if (!firstCrossing_) {
      cyclesCompleted_++;
    }

    if (cyclesCompleted_ > 2) {
      if (valleyTemp_ < valleyAmplitudeSum_) valleyAmplitudeSum_ = valleyTemp_;
    }
    peakTemp_ = input;
    phase_ = Phase::RELAY_HIGH;
    firstCrossing_ = false;
  }

  if (measurementCount_ >= config_.requiredCycles) {
    computeTunings();
    result_ = Result::SUCCESS;
    phase_ = Phase::COMPLETE;
  }

  return heaterOn ? static_cast<float>(config_.heaterHigh) : static_cast<float>(config_.heaterLow);
}

void AutoTuner::start() {
  reset();
  phase_ = Phase::RELAY_HIGH;
  totalStartTime_ = millis();
  phaseStartTime_ = millis();
  result_ = Result::ERROR;
}

void AutoTuner::cancel() {
  phase_ = Phase::COMPLETE;
  result_ = Result::CANCELLED;
}

void AutoTuner::reset() {
  phase_ = Phase::IDLE;
  result_ = Result::ERROR;
  tunings_ = {0.0f, 0.0f, 0.0f};
  lastHeaterPwm_ = 0;
  phaseStartTime_ = 0;
  totalStartTime_ = 0;
  cyclesCompleted_ = 0;
  peakTemp_ = -1000.0f;
  valleyTemp_ = 1000.0f;
  lastPeakTime_ = 0;
  lastValleyTime_ = 0;
  lastError_ = 0.0f;
  firstCrossing_ = true;
  peakPeriodSum_ = 0;
  measurementCount_ = 0;
  peakAmplitudeSum_ = -1000.0f;
  valleyAmplitudeSum_ = 1000.0f;
  valleyAmplitudeNext_ = 1000.0f;
}

bool AutoTuner::isRunning() const {
  return phase_ == Phase::RELAY_HIGH || phase_ == Phase::RELAY_LOW;
}

bool AutoTuner::isComplete() const {
  return phase_ == Phase::COMPLETE;
}

AutoTuner::Phase AutoTuner::getPhase() const {
  return phase_;
}

AutoTuner::Result AutoTuner::getResult() const {
  return result_;
}

PIDTunings AutoTuner::getTunings() const {
  return tunings_;
}

float AutoTuner::getProgress() const {
  if (phase_ == Phase::IDLE) return 0.0f;
  if (phase_ == Phase::COMPLETE) return 1.0f;
  float totalHalfCycles = 2 + config_.requiredCycles * 2; // 2 stabilize + N measurement
  return min(static_cast<float>(cyclesCompleted_) / totalHalfCycles, 0.99f);
}

void AutoTuner::computeTunings() {
  // Relay feedback: Ku = 4a / (pi * A)
  // a = relay half-amplitude, A = peak-to-peak process output amplitude
  float relayAmp = static_cast<float>(config_.heaterHigh - config_.heaterLow) / 2.0f;
  float peakToPeakAmp = peakAmplitudeSum_ - valleyAmplitudeSum_;
  if (peakToPeakAmp < 0.01f) peakToPeakAmp = 0.01f;

  float Ku = (4.0f * relayAmp) / (PI * peakToPeakAmp);

  // Ultimate period in seconds
  float Tu = static_cast<float>(peakPeriodSum_) / (measurementCount_ * 1000.0f);
  if (Tu < 0.1f) Tu = 20.0f;

  // Ziegler-Nichols PID
  tunings_.kp = 0.6f * Ku;
  tunings_.ki = tunings_.kp / (Tu * 0.5f);
  tunings_.kd = tunings_.kp * Tu / 8.0f;
}

void AutoTuner::resetTracking() {
  peakTemp_ = -1000.0f;
  valleyTemp_ = 1000.0f;
  lastPeakTime_ = 0;
  lastValleyTime_ = 0;
  firstCrossing_ = true;
  peakPeriodSum_ = 0;
  measurementCount_ = 0;
  peakAmplitudeSum_ = -1000.0f;
  valleyAmplitudeSum_ = 1000.0f;
  valleyAmplitudeNext_ = 1000.0f;
}