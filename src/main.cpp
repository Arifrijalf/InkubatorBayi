#include <ESPmDNS.h>
#include <Arduino.h>
#include "config/system_config.h"
#include "utils/types.h"
#include "storage/preferences_store.h"
#include "sensor/temperature_sensor.h"
#include "actuator/heater.h"
#include "actuator/fan.h"
#include "actuator/buzzer.h"
#include "control/pid_controller.h"
#include "communication/wifi_manager.h"
#include "communication/websocket_handler.h"
#include "communication/web_server.h"
#include "dashboard/dashboard_data.h"

static PreferencesStore storage;
static TemperatureSensor sensor;
static Heater heater;
static Fan fan;
static Buzzer buzzer;
static PIDController pid;
static WiFiManager wifi;
static WebSocketHandler ws;
static HttpServer httpServer;
static DashboardData dashData;

static SystemData sysData;
static uint32_t lastBroadcast = 0;
static uint32_t lastUpdate = 0;

static void onConfigUpdate(const PIDTunings& t, float sp) {
  pid.setTunings(t);
  pid.setSetpoint(sp);
  sysData.pid = t;
  sysData.setpoint = sp;
  storage.savePIDTunings(t);
  storage.saveSetpoint(sp);
}

static void onWiFiUpdate(const String& ssid, const String& pass) {
  storage.saveWiFiCredentials(ssid, pass);
  ESP.restart();
}

void setup() {
  Serial.begin(115200);
  delay(100);

  btStop();
  
  storage.begin();
  
  PIDTunings tunings = storage.loadPIDTunings();
  float setpoint = storage.loadSetpoint();
  
  sysData.pid = tunings;
  sysData.setpoint = setpoint;
  sysData.state = SystemState::INIT;
  
  sensor.begin();
  heater.begin();
  fan.begin();
  buzzer.begin();
  
  pid.begin(tunings, setpoint);
  
  // DHCP setup (Aa)
  // wifi.begin("Aa", "90909091", INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  
  // Static IP setup (ocillll)
  IPAddress ip(192, 168, 0, 115);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress gateway(192, 168, 0, 1);
  IPAddress dns(8, 8, 8, 8);
  
  String ssid, pass;
  storage.loadWiFiCredentials(ssid, pass); 
  wifi.addFallback("Aa", "90909091"); 
  wifi.begin(ssid, pass, ip, subnet, gateway, dns);
  
  dashData.setSystemData(&sysData);
  
  httpServer.setSystemData(&sysData);
  httpServer.setTuningsCallback(onConfigUpdate);
  httpServer.setWiFiCallback(onWiFiUpdate);
  httpServer.begin();

  if (MDNS.begin("inkubator")) {
    MDNS.addService("http", "tcp", 80);
    Serial.println("[MDNS] http://inkubator.local");
  }

  ws.begin(httpServer.getServer(), &sysData, onConfigUpdate);
  
  sysData.state = SystemState::NORMAL;
  
  Serial.println("[MAIN] System initialized");
}

void loop() {
  uint32_t now = millis();
  
  sensor.update();
  wifi.update();
  ws.cleanup();
  
  sysData.sensor.temperature = sensor.getTemperature();
  sysData.sensor.status = sensor.getStatus();
  sysData.wifiRssi = wifi.getRSSI();
  sysData.freeHeap = ESP.getFreeHeap();
  sysData.uptime = now / 1000;
  
  if (now - lastUpdate >= Config::PID_SAMPLE_TIME_MS) {
    lastUpdate = now;
    
    float temp = sensor.getTemperature();
    float pidOutput = pid.compute(temp);
    
    sysData.control.pidResult = pidOutput;
    sysData.control.error = sysData.setpoint - temp;
    
    if (sensor.isConnected()) {
      sysData.state = SystemState::NORMAL;
      if (pidOutput > 0) {
        heater.setPWM(static_cast<int>(pidOutput));
        fan.stop();
      } else {
        heater.stop();
        fan.setPWM(static_cast<int>(-pidOutput));
      }
    } else {
      heater.stop();
      fan.setPWM(PWM::Fan::MAX_VALUE);
      sysData.state = SystemState::SENSOR_FAULT;
    }
    
    sysData.control.heaterPwm = heater.getCurrentPwm();
    sysData.control.fanPwm = fan.getCurrentPwm();
    
    bool alarm = false;
    if (sensor.isConnected() && temp > sysData.setpoint + Config::ALARM_OVERTEMP_OFFSET) {
      alarm = true;
    }
    buzzer.update(alarm);
    sysData.alarm = alarm;
  }
  
  if (now - lastBroadcast >= Config::WS_BROADCAST_INTERVAL_MS) {
    lastBroadcast = now;
    ws.broadcast(sysData);
  }
}
