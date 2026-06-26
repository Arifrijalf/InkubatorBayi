#include "communication/wifi_manager.h"

void WiFiManager::begin(const String& ssid, const String& pass) {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  connectSTA(ssid, pass);
}

void WiFiManager::connectSTA(const String& ssid, const String& pass) {
  WiFi.begin(ssid.c_str(), pass.c_str());
  
  uint8_t retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < Config::MAX_WIFI_RETRY) {
    delay(Config::WIFI_RETRY_DELAY_MS);
    retries++;
  }
  
  if (WiFi.status() != WL_CONNECTED) {
    startAP();
  }
}

void WiFiManager::startAP() {
  apMode_ = true;
  WiFi.mode(WIFI_AP);
  WiFi.softAP(Config::AP_SSID, Config::AP_PASS);
}

void WiFiManager::update() {
  if (!apMode_) {
    if (WiFi.status() != WL_CONNECTED) {
      uint32_t now = millis();
      if (now - lastReconnectAttempt_ > 5000) {
        lastReconnectAttempt_ = now;
        WiFi.reconnect();
      }
    }
  }
}

void WiFiManager::disconnect() {
  WiFi.disconnect();
}

bool WiFiManager::isConnected() const {
  if (apMode_) return true;
  return WiFi.status() == WL_CONNECTED;
}

bool WiFiManager::isAPMode() const {
  return apMode_;
}

int WiFiManager::getRSSI() const {
  return apMode_ ? 0 : WiFi.RSSI();
}

String WiFiManager::getIP() const {
  return apMode_ ? WiFi.softAPIP().toString() : WiFi.localIP().toString();
}

String WiFiManager::getSSID() const {
  return WiFi.SSID();
}
