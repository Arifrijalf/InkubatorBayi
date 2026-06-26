#include "sensor/temperature_sensor.h"
#include <Arduino.h>

void TemperatureSensor::begin() {
  oneWire_.begin(Pin::ONE_WIRE_BUS);
  sensors_.setOneWire(&oneWire_);
  sensors_.begin();
  sensors_.setResolution(12);
  data_.status = SensorStatus::READING;
  requestTemperature();
}

void TemperatureSensor::update() {
  uint32_t now = millis();

  if (waitingForConversion_) {
    if (now - lastRequestTime_ < 750) return;
    
    if (readTemperature()) {
      data_.status = SensorStatus::OK;
    } else {
      if (data_.status == SensorStatus::OK) {
        data_.status = SensorStatus::DISCONNECTED;
      }
    }
    waitingForConversion_ = false;
    data_.lastReadTime = now;
    requestTemperature();
    return;
  }

  if (now - data_.lastReadTime >= Config::SENSOR_READ_INTERVAL_MS) {
    requestTemperature();
  }
}

void TemperatureSensor::requestTemperature() {
  sensors_.requestTemperatures();
  lastRequestTime_ = millis();
  waitingForConversion_ = true;
}

bool TemperatureSensor::readTemperature() {
  float temp = sensors_.getTempCByIndex(DEVICE_INDEX);
  
  if (temp == INVALID_RESULT || temp < Config::TEMP_FAULT_LOW || temp > Config::TEMP_FAULT_HIGH) {
    return false;
  }
  
  data_.temperature = temp;
  lastValidTemp_ = temp;
  return true;
}

float TemperatureSensor::getTemperature() const {
  if (data_.status == SensorStatus::OK) {
    return data_.temperature;
  }
  return lastValidTemp_;
}

SensorStatus TemperatureSensor::getStatus() const {
  return data_.status;
}

bool TemperatureSensor::isConnected() const {
  return data_.status == SensorStatus::OK;
}
