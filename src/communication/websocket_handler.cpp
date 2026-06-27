#include <ESPAsyncWebServer.h>
#include "communication/websocket_handler.h"

void WebSocketHandler::begin(AsyncWebServer& server, SystemData* data, void (*cb)(const PIDTunings&, float)) {
  sysData_ = data;
  tuningsCallback_ = cb;
  ws_.onEvent([this](AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
    handleEvent(server, client, type, arg, data, len);
  });
  server.addHandler(&ws_);
}

void WebSocketHandler::handleEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
  if (type == WS_EVT_DATA) {
    AwsFrameInfo* info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
      String msg = "";
      for (size_t i = 0; i < len; i++) msg += (char)data[i];
      processMessage(msg);
    }
  }
}

void WebSocketHandler::broadcast(const SystemData& data) {
  if (ws_.count() == 0) return;
  String json = serializeSystemData(data);
  ws_.textAll(json);
}

String WebSocketHandler::serializeSystemData(const SystemData& data) {
  JsonDocument doc;
  doc["temperature"] = round(data.sensor.temperature * 100.0f) / 100.0f;
  doc["setpoint"] = round(data.setpoint * 100.0f) / 100.0f;
  doc["heater_pwm"] = data.control.heaterPwm;
  doc["fan_pwm"] = data.control.fanPwm;
  doc["pid_output"] = round(data.control.pidResult * 100.0f) / 100.0f;
  doc["error"] = round(data.control.error * 100.0f) / 100.0f;
  doc["kp"] = round(data.pid.kp * 100.0f) / 100.0f;
  doc["ki"] = round(data.pid.ki * 100.0f) / 100.0f;
  doc["kd"] = round(data.pid.kd * 100.0f) / 100.0f;
  doc["wifi"] = data.wifiRssi;
  doc["heap"] = data.freeHeap;
  doc["uptime"] = data.uptime;
  doc["sensor_status"] = static_cast<int>(data.sensor.status);
  doc["state"] = static_cast<int>(data.state);
  doc["alarm"] = data.alarm;
  String output;
  serializeJson(doc, output);
  return output;
}

void WebSocketHandler::processMessage(const String& msg) {
  if (!sysData_) return;
  
  Serial.printf("[WS] Pesan: %s\r\n", msg.c_str());
  
  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, msg);
  if (err) {
    Serial.printf("[WS] JSON error: %s\r\n", err.c_str());
    return;
  }
  
  PIDTunings t = sysData_->pid;
  float sp = sysData_->setpoint;
  bool changed = false;

  if (doc["setpoint"].is<double>()) {
    sp = doc["setpoint"].as<float>();
    changed = true;
  }
  if (doc["kp"].is<double>()) { t.kp = doc["kp"].as<float>(); changed = true; }
  if (doc["ki"].is<double>()) { t.ki = doc["ki"].as<float>(); changed = true; }
  if (doc["kd"].is<double>()) { t.kd = doc["kd"].as<float>(); changed = true; }
  
  Serial.printf("[WS] SP=%.1f Kp=%.2f Ki=%.2f Kd=%.2f\r\n", sp, t.kp, t.ki, t.kd);
  
  if (changed && tuningsCallback_) {
    tuningsCallback_(t, sp);
  }
}

uint32_t WebSocketHandler::getConnectionCount() const { return ws_.count(); }
void WebSocketHandler::cleanup() { ws_.cleanupClients(); }
