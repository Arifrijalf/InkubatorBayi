#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include "config/system_config.h"
#include "utils/types.h"

class WebSocketHandler {
public:
  void begin(AsyncWebServer& server, SystemData* data, void (*cb)(const PIDTunings&, float));
  void setTunerCallback(void (*cb)(bool start));
  void broadcast(const SystemData& data);
  
  uint32_t getConnectionCount() const;
  void cleanup();

private:
  AsyncWebSocket ws_{"/ws"};
  SystemData* sysData_ = nullptr;
  void (*tuningsCallback_)(const PIDTunings&, float) = nullptr;
  void (*tunerCallback_)(bool) = nullptr;

  void handleEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len);
  String serializeSystemData(const SystemData& data);
  void processMessage(const String& msg);
};
