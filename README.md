# Inkubator Bayi — PID Temperature Monitoring & Control

ESP32-based neonatal incubator temperature controller with real-time web dashboard.

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
| DS18B20 Data | GPIO 4 |
| Heater MOSFET | GPIO 16 |
| Fan MOSFET | GPIO 17 |

## Features

- Manual PID implementation (no libraries)
- Integral anti-windup
- Derivative filtering
- Non-blocking sensor reads
- Sensor fault detection with auto-safety response
- Real-time WebSocket dashboard
- WiFi STA + AP fallback
- Persistent storage (ESP32 Preferences)
- Dark mode glassmorphism UI
- Live temperature chart (Chart.js)

## Dashboard

- Real-time temperature, setpoint, PID output
- Heater & Fan PWM gauges
- Kp / Ki / Kd / Setpoint adjustment
- WiFi RSSI, free heap, uptime
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
  sensor/         DS18B20 driver
  actuator/       Heater, Fan PWM
  control/        PID controller
  communication/  WiFi, WebSocket, HTTP
  dashboard/      Data aggregator, HTML
```
