#pragma once

#include "utils/types.h"

class DashboardData {
public:
  void setSystemData(SystemData* data);
  void update(float temperature, float pidOutput, int heaterPwm, int fanPwm);
  void updateWiFi(int rssi);
  const SystemData& getData() const;

private:
  SystemData* sysData_ = nullptr;
};
