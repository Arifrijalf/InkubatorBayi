#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include "config/system_config.h"
#include "utils/types.h"

class WebSocketHandler {
public:
  void begin(AsyncWebServer& server);
  void broadcast(const SystemData& data);
  void onMessage(const String& msg, SystemData& data);
  
  uint32_t getConnectionCount() const;
  void cleanup();

private:
  AsyncWebSocket ws_{"/ws"};
  
  String serializeSystemData(const SystemData& data);
  void processMessage(const String& msg, SystemData& data);
};
