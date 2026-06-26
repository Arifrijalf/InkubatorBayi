# AGENTS.md

# Inkubator Bayi PID Monitoring & Control

## Project Overview

This project develops an IoT-based neonatal incubator monitoring and control system using ESP32.

The system controls the incubator temperature using a PID controller and provides a modern web dashboard for monitoring and configuration.

Main objectives:

- Stable temperature control
- Real-time monitoring
- PID parameter tuning from web
- Responsive dashboard
- Clean and maintainable codebase

---

# Hardware

MCU

- ESP32 DevKit V1

Sensor

- DS18B20

Actuator

- Air Heater 12V DC
- Cooling Fan 12V DC

Driver

- IRF520 MOSFET Module

Power

- LM2596 Buck Converter

Communication

- WiFi
- WebSocket
- HTTP

---

# Software Stack

PlatformIO

Framework:
Arduino

Language:
C++17

Dashboard

HTML

CSS

JavaScript

Chart.js

Communication

AsyncWebServer

AsyncTCP

ArduinoJson

Storage

Preferences

---

# Code Style

Always follow these rules.

- SOLID principles
- Single Responsibility Principle
- No giant main.cpp
- Maximum function length around 40 lines
- Maximum file length around 300 lines if possible
- Meaningful variable names
- Avoid global variables whenever possible
- Use constexpr instead of #define where applicable
- Use enum class instead of magic numbers
- Use comments only when necessary

---

# Folder Structure

src/

    main.cpp

    config/

    sensor/

    control/

    actuator/

    communication/

    dashboard/

    storage/

    utils/

data/

    index.html

    css/

    js/

include/

lib/

---

# Modules

## Sensor

Responsible only for reading sensors.

Never calculate PID here.

Functions

- begin()
- update()
- getTemperature()

---

## PID Controller

Responsible only for PID computation.

Functions

- begin()
- compute()
- setTunings()
- setSetpoint()
- getOutput()

No hardware access inside PID module.

---

## Heater

Responsible only for heater PWM.

Functions

- begin()
- setPWM()
- stop()

---

## Fan

Responsible only for fan PWM.

Functions

- begin()
- setPWM()
- stop()

---

## WiFi Manager

Responsible only for

- WiFi connection
- Reconnection
- WiFi status

---

## Web Server

Responsible only for

- HTTP Routes
- Static files
- REST APIs

Never place PID logic here.

---

## WebSocket

Responsible only for

- Real-time communication

No hardware access.

---

## Storage

Responsible only for

Saving

- Kp
- Ki
- Kd
- Setpoint

using Preferences.

---

# Dashboard

Dashboard must display

Temperature

Setpoint

Heater PWM

Fan PWM

PID Output

Error

Kp

Ki

Kd

WiFi Status

RSSI

Free Heap

Uptime

Realtime Temperature Chart

---

# Dashboard Requirements

Modern UI

Responsive

Dark mode

Glassmorphism style

Smooth animations

No page refresh

Realtime updates using WebSocket

---

# Communication

Dashboard ←→ ESP32

Use WebSocket.

HTTP only for

- configuration
- static files

Realtime data must never use polling.

---

# PID

PID should be implemented manually.

Do NOT use PID libraries.

Algorithm

Error

Integral

Derivative

Output

Include

Output limits

Integral windup protection

Derivative filtering

Sample time

---

# PWM

Use LEDC API.

Heater

5kHz

10-bit

Fan

25kHz

10-bit

---

# Data Format

WebSocket JSON example

{
"temperature":36.85,
"setpoint":37.0,
"heater_pwm":175,
"fan_pwm":80,
"pid_output":175,
"error":0.15,
"kp":2.0,
"ki":0.8,
"kd":1.2,
"heap":225000,
"wifi":-48,
"uptime":325
}

---

# Future Features

Design every module so future features can be added easily.

Examples

- OTA Update
- MQTT
- SD Card Logging
- Firebase
- Auto PID Tuning
- Alarm System
- LCD
- Touchscreen
- Multiple Sensors

Avoid hardcoded values.

Everything configurable should be stored in Preferences.

---

# Safety

The controller must never output invalid PWM values.

Clamp PWM inside allowed range.

Detect sensor failure.

If sensor disconnected:

- Heater OFF
- Fan ON
- Send alarm to dashboard

---

# Development Rules

Before creating new files:

1. Check if a similar module already exists.

2. Reuse code whenever possible.

3. Keep modules independent.

4. Never duplicate logic.

5. Explain architectural decisions when introducing significant changes.

6. Every new feature must not break existing APIs.

7. Prefer readability over clever optimizations.

8. Every class should have one responsibility.

9. Every commit should compile successfully.

10. Write production-quality code.
