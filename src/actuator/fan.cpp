#include "actuator/fan.h"

void Fan::begin() {
  ledcSetup(PWM::Fan::CHANNEL, PWM::Fan::FREQ_HZ, PWM::Fan::RESOLUTION_BITS);
  ledcAttachPin(Pin::FAN_PWM, PWM::Fan::CHANNEL);
  stop();
}

void Fan::setPWM(int value) {
  int clamped = constrain(value, 0, static_cast<int>(PWM::Fan::MAX_VALUE));
  if (clamped != currentPwm_) {
    ledcWrite(PWM::Fan::CHANNEL, clamped);
    currentPwm_ = clamped;
  }
}

void Fan::stop() {
  ledcWrite(PWM::Fan::CHANNEL, 0);
  currentPwm_ = 0;
}

int Fan::getCurrentPwm() const {
  return currentPwm_;
}
