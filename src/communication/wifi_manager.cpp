#include "communication/wifi_manager.h"

void WiFiManager::begin(const String& ssid, const String& pass, const IPAddress& ip, const IPAddress& subnet, const IPAddress& gateway, const IPAddress& dns) {
  Serial.println("[WiFi] Memulai koneksi...");
  Serial.printf("[WiFi] SSID primer: %s\r\n", ssid.c_str());
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  connectSTA(ssid, pass, ip, subnet, gateway, dns);
  
  if (isConnected()) {
    Serial.printf("[WiFi] Terhubung! IP: %s\r\n", getIP().c_str());
    Serial.printf("[WiFi] RSSI: %d dBm\r\n", getRSSI());
  } else if (isAPMode()) {
    Serial.printf("[WiFi] Semua gagal. Mode AP: %s | IP: %s\r\n", getSSID().c_str(), getIP().c_str());
  } else {
    Serial.println("[WiFi] Gagal terhubung - tidak ada koneksi");
  }
}

void WiFiManager::addFallback(const String& ssid, const String& pass) {
  fallbackSsid_ = ssid;
  fallbackPass_ = pass;
  Serial.printf("[WiFi] Fallback diset: %s\r\n", ssid.c_str());
}

void WiFiManager::addFallback2(const String& ssid, const String& pass) {
  fallbackSsid2_ = ssid;
  fallbackPass2_ = pass;
  Serial.printf("[WiFi] Fallback2 diset: %s\r\n", ssid.c_str());
}

void WiFiManager::connectSTA(const String& ssid, const String& pass, const IPAddress& ip, const IPAddress& subnet, const IPAddress& gateway, const IPAddress& dns) {
  if (ip != INADDR_NONE) {
    if (!WiFi.config(ip, gateway, subnet, dns)) {
      Serial.println("[WiFi] WiFi.config gagal");
    }
  }
  WiFi.begin(ssid.c_str(), pass.c_str());
  
  uint8_t retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < Config::MAX_WIFI_RETRY) {
    Serial.printf("[WiFi] Menunggu koneksi... (%d/%d)\r\n", retries + 1, Config::MAX_WIFI_RETRY);
    delay(Config::WIFI_RETRY_DELAY_MS);
    retries++;
  }
  
  if (WiFi.status() != WL_CONNECTED && fallbackSsid_.length() > 0) {
    Serial.printf("[WiFi] Gagal ke %s, mencoba fallback ke %s\r\n", ssid.c_str(), fallbackSsid_.c_str());
    WiFi.disconnect();
    delay(100);
    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE);
    WiFi.begin(fallbackSsid_.c_str(), fallbackPass_.c_str());
    
    retries = 0;
    while (WiFi.status() != WL_CONNECTED && retries < Config::MAX_WIFI_RETRY) {
      Serial.printf("[WiFi] Menunggu fallback 1... (%d/%d)\r\n", retries + 1, Config::MAX_WIFI_RETRY);
      delay(Config::WIFI_RETRY_DELAY_MS);
      retries++;
    }
  }
  
  if (WiFi.status() != WL_CONNECTED && fallbackSsid2_.length() > 0) {
    Serial.printf("[WiFi] Semua gagal, mencoba fallback2 ke %s\r\n", fallbackSsid2_.c_str());
    WiFi.disconnect();
    delay(100);
    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE);
    WiFi.begin(fallbackSsid2_.c_str(), fallbackPass2_.c_str());
    
    retries = 0;
    while (WiFi.status() != WL_CONNECTED && retries < Config::MAX_WIFI_RETRY) {
      Serial.printf("[WiFi] Menunggu fallback 2... (%d/%d)\r\n", retries + 1, Config::MAX_WIFI_RETRY);
      delay(Config::WIFI_RETRY_DELAY_MS);
      retries++;
    }
  }
  
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[WiFi] Semua koneksi gagal, beralih ke AP mode");
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
        Serial.println("[WiFi] Mencoba reconnect...");
        WiFi.reconnect();
      }
    } else if (WiFi.status() == WL_CONNECTED && lastReconnectAttempt_ != 0) {
      Serial.printf("[WiFi] Reconnect berhasil! IP: %s\r\n", WiFi.localIP().toString().c_str());
      lastReconnectAttempt_ = 0;
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
