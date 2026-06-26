#include <ESPAsyncWebServer.h>
#include "communication/websocket_handler.h"

void WebSocketHandler::begin(AsyncWebServer& server) {
  server.addHandler(&ws_);
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

void WebSocketHandler::onMessage(const String& msg, SystemData& data) {
  processMessage(msg, data);
}

void WebSocketHandler::processMessage(const String& msg, SystemData& data) {
  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, msg);
  
  if (err) return;
  
  if (doc["setpoint"].is<float>()) {
    float sp = doc["setpoint"].as<float>();
    if (sp >= Config::TEMP_MIN && sp <= Config::TEMP_MAX) {
      data.setpoint = sp;
    }
  }
  
  if (doc["kp"].is<float>()) {
    data.pid.kp = doc["kp"].as<float>();
  }
  if (doc["ki"].is<float>()) {
    data.pid.ki = doc["ki"].as<float>();
  }
  if (doc["kd"].is<float>()) {
    data.pid.kd = doc["kd"].as<float>();
  }
}

uint32_t WebSocketHandler::getConnectionCount() const {
  return ws_.count();
}

void WebSocketHandler::cleanup() {
  ws_.cleanupClients();
}
