#include <Arduino.h>
#include "dashboard/dashboard_data.h"

void DashboardData::setSystemData(SystemData* data) {
  sysData_ = data;
}

void DashboardData::update(float temperature, float pidOutput, int heaterPwm, int fanPwm) {
  if (!sysData_) return;
  
  sysData_->sensor.temperature = temperature;
  sysData_->control.pidResult = pidOutput;
  sysData_->control.heaterPwm = heaterPwm;
  sysData_->control.fanPwm = fanPwm;
  sysData_->control.error = sysData_->setpoint - temperature;
  sysData_->freeHeap = ESP.getFreeHeap();
  sysData_->uptime = millis() / 1000;
}

void DashboardData::updateWiFi(int rssi) {
  if (!sysData_) return;
  sysData_->wifiRssi = rssi;
}

const SystemData& DashboardData::getData() const {
  static SystemData empty;
  return sysData_ ? *sysData_ : empty;
}
