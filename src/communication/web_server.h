#pragma once

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "utils/types.h"
#include "dashboard/dashboard_html.h"

class HttpServer {
public:
  HttpServer();
  void begin();
  AsyncWebServer& getServer();
  
  void setSystemData(SystemData* data);
  void setTuningsCallback(void (*cb)(const PIDTunings&, float));
  void setWiFiCallback(void (*cb)(const String&, const String&));

private:
  AsyncWebServer server_;
  SystemData* sysData_ = nullptr;
  
  void (*tuningsCallback_)(const PIDTunings&, float) = nullptr;
  void (*wifiCallback_)(const String&, const String&) = nullptr;
  
  void setupRoutes();
  void handleGetConfig(AsyncWebServerRequest* req);
  void handlePostConfig(AsyncWebServerRequest* req, uint8_t* data, size_t len, size_t index, size_t total);
  void handleGetWiFi(AsyncWebServerRequest* req);
  void handlePostWiFi(AsyncWebServerRequest* req, uint8_t* data, size_t len, size_t index, size_t total);
};
