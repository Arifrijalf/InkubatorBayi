# Inkubator Bayi — Neonatal Incubator PID Temperature Controller

ESP32-based neonatal incubator temperature controller with real-time web dashboard, alarm system, auto-tuning, and emergency switch.

[![Build Status](https://img.shields.io/badge/PlatformIO-Ready-green?style=flat-square&logo=platformio)](https://platformio.org)
[![Temperature Control](https://img.shields.io/badge/PID%20Control-Stable-blue?style=flat-square)](https://github.com/AnomalyResearch/InkubatorBayi)

## Hardware

| Component | Specification |
|---|---|
| MCU | ESP32 DevKit V1 |
| Sensor | DS18B20 (waterproof) |
| Heater | 12V DC Air Heater |
| Fan | 12V DC Cooling Fan |
| Driver | IRF520 MOSFET Module |
| Power | LM2596 Buck Converter (12V→5V) |

### Wiring

| Component | GPIO |
|---|---|
| DS18B20 Data | GPIO 19 |
| Heater MOSFET | GPIO 21 |
| Fan MOSFET | GPIO 22 |
| Buzzer | GPIO 23 |
| Normal Indicator | GPIO 26 |
| Emergency Switch | GPIO 27 |

## Features

- Manual PID implementation
- Integral anti-windup
- Derivative filtering
- Non-blocking sensor reads
- Sensor fault detection with auto-safety response
- Emergency physical switch for immediate heater shutdown and fan activation
- Buzzer alarm for critical events (e.g., sensor fault, emergency switch active)
- Real-time WebSocket dashboard
- WiFi STA + AP fallback
- Persistent storage (ESP32 Preferences)
- Dark mode glassmorphism UI
- Live temperature chart (Chart.js)

## Dashboard

- Real-time temperature, setpoint, PID output, error
- Heater & Fan PWM gauges
- Kp / Ki / Kd / Setpoint adjustment
- Auto-tune start/stop and progress display
- WiFi RSSI, free heap, uptime
- Buzzer alarm status
- Emergency switch status
- Live temperature history graph

## Build & Flash

```bash
pio run -t upload
pio device monitor
```

## WiFi

- **STA mode**: Connects to saved network
- **AP fallback**: `InkubatorAP` / `12345678`
- Open `http://<ESP32_IP>` for dashboard

## PID Defaults

| Parameter | Value |
|---|---|
| Kp | 2.0 |
| Ki | 0.5 |
| Kd | 1.0 |
| Setpoint | 37.0°C |
| Sample Time | 500ms |
| Output Range | 0–1023 |

## Project Structure

```
src/
  main.cpp
  config/         pin_config, system_config
  utils/          types (enums, structs)
  storage/        Preferences read/write
  sensor/         DS18B20 driver, emergency switch
  actuator/       Heater, Fan, Buzzer PWM
  control/        PID controller, auto-tuner
  communication/  WiFi, WebSocket, HTTP
  dashboard/      Data aggregator, HTML
```
