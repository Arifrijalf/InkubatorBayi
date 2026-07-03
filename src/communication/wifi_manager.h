#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "config/system_config.h"

class WiFiManager {
public:
  void begin(const String& ssid, const String& pass, const IPAddress& ip, const IPAddress& subnet, const IPAddress& gateway, const IPAddress& dns);
  void addFallback(const String& ssid, const String& pass);
  void addFallback2(const String& ssid, const String& pass);
  void update();
  void disconnect();
  
  bool isConnected() const;
  bool isAPMode() const;
  int getRSSI() const;
  String getIP() const;
  String getSSID() const;

private:
  AsyncWebServer* server_ = nullptr;
  bool apMode_ = false;
  uint32_t lastReconnectAttempt_ = 0;
  String fallbackSsid_;
  String fallbackPass_;
  String fallbackSsid2_;
  String fallbackPass2_;

  void startAP();
  void connectSTA(const String& ssid, const String& pass, const IPAddress& ip, const IPAddress& subnet, const IPAddress& gateway, const IPAddress& dns);
  void setupDNS();
};
