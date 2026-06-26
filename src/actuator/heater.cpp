#include "actuator/heater.h"

void Heater::begin() {
  ledcSetup(PWM::Heater::CHANNEL, PWM::Heater::FREQ_HZ, PWM::Heater::RESOLUTION_BITS);
  ledcAttachPin(Pin::HEATER_PWM, PWM::Heater::CHANNEL);
  stop();
}

void Heater::setPWM(int value) {
  int clamped = constrain(value, 0, static_cast<int>(PWM::Heater::MAX_VALUE));
  if (clamped != currentPwm_) {
    ledcWrite(PWM::Heater::CHANNEL, clamped);
    currentPwm_ = clamped;
  }
}

void Heater::stop() {
  ledcWrite(PWM::Heater::CHANNEL, 0);
  currentPwm_ = 0;
}

int Heater::getCurrentPwm() const {
  return currentPwm_;
}
