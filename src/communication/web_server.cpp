#include <Arduino.h>
#include "communication/web_server.h"
#include "config/system_config.h"

HttpServer::HttpServer() : server_(Config::WEB_SERVER_PORT) {}

void HttpServer::begin() {
  setupRoutes();
  server_.begin();
}

AsyncWebServer& HttpServer::getServer() {
  return server_;
}

void HttpServer::setSystemData(SystemData* data) {
  sysData_ = data;
}

void HttpServer::setTuningsCallback(void (*cb)(const PIDTunings&, float)) {
  tuningsCallback_ = cb;
}

void HttpServer::setWiFiCallback(void (*cb)(const String&, const String&)) {
  wifiCallback_ = cb;
}

void HttpServer::setupRoutes() {
  server_.on("/", HTTP_GET, [this](AsyncWebServerRequest* req) {
    req->send(200, "text/html", INDEX_HTML);
  });

  server_.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest* req) {
    req->send(204);
  });
  
  server_.on("/api/config", HTTP_GET, [this](AsyncWebServerRequest* req) {
    handleGetConfig(req);
  });
  
  server_.on("/api/config", HTTP_POST, 
    [this](AsyncWebServerRequest* req) {},
    nullptr,
    [this](AsyncWebServerRequest* req, uint8_t* data, size_t len, size_t index, size_t total) {
      handlePostConfig(req, data, len, index, total);
    });
  
  server_.on("/api/wifi", HTTP_GET, [this](AsyncWebServerRequest* req) {
    handleGetWiFi(req);
  });
  
  server_.on("/api/wifi", HTTP_POST,
    [this](AsyncWebServerRequest* req) {},
    nullptr,
    [this](AsyncWebServerRequest* req, uint8_t* data, size_t len, size_t index, size_t total) {
      handlePostWiFi(req, data, len, index, total);
    });
}

void HttpServer::handleGetConfig(AsyncWebServerRequest* req) {
  if (!sysData_) {
    req->send(500, "application/json", "{\"error\":\"no data\"}");
    return;
  }
  
  JsonDocument doc;
  doc["kp"] = sysData_->pid.kp;
  doc["ki"] = sysData_->pid.ki;
  doc["kd"] = sysData_->pid.kd;
  doc["setpoint"] = sysData_->setpoint;
  
  String json;
  serializeJson(doc, json);
  req->send(200, "application/json", json);
}

void HttpServer::handlePostConfig(AsyncWebServerRequest* req, uint8_t* data, size_t len, size_t index, size_t total) {
  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, data, len);
  
  if (err || !sysData_) {
    req->send(400, "application/json", "{\"error\":\"invalid json\"}");
    return;
  }
  
  PIDTunings t = sysData_->pid;
  float sp = sysData_->setpoint;
  
  if (doc["kp"].is<float>()) t.kp = doc["kp"].as<float>();
  if (doc["ki"].is<float>()) t.ki = doc["ki"].as<float>();
  if (doc["kd"].is<float>()) t.kd = doc["kd"].as<float>();
  if (doc["setpoint"].is<float>()) sp = doc["setpoint"].as<float>();
  
  if (tuningsCallback_) {
    tuningsCallback_(t, sp);
  }
  
  req->send(200, "application/json", "{\"ok\":true}");
}

void HttpServer::handleGetWiFi(AsyncWebServerRequest* req) {
  JsonDocument doc;
  doc["connected"] = WiFi.status() == WL_CONNECTED;
  doc["ip"] = WiFi.localIP().toString();
  doc["rssi"] = WiFi.RSSI();
  
  String json;
  serializeJson(doc, json);
  req->send(200, "application/json", json);
}

void HttpServer::handlePostWiFi(AsyncWebServerRequest* req, uint8_t* data, size_t len, size_t index, size_t total) {
  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, data, len);
  
  if (err || !wifiCallback_) {
    req->send(400, "application/json", "{\"error\":\"invalid json\"}");
    return;
  }
  
  String ssid = doc["ssid"] | "";
  String pass = doc["password"] | "";
  
  if (ssid.length() > 0) {
    wifiCallback_(ssid, pass);
  }
  
  req->send(200, "application/json", "{\"ok\":true}");
}
