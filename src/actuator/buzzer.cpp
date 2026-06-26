#include "actuator/buzzer.h"

void Buzzer::begin() {
  pinMode(Pin::BUZZER, OUTPUT);
  off();
}

void Buzzer::on() {
  if (!active_) {
    digitalWrite(Pin::BUZZER, HIGH);
    active_ = true;
  }
}

void Buzzer::off() {
  if (active_) {
    digitalWrite(Pin::BUZZER, LOW);
    active_ = false;
  }
}

void Buzzer::update(bool alarmActive) {
  if (alarmActive) {
    on();
  } else {
    off();
  }
}

bool Buzzer::isOn() const {
  return active_;
}
