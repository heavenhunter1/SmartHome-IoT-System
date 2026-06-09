# Demo Checklist

This document is used to prepare a short demo video for the Smart Home IoT System.

The goal of the demo is to show that the system works as a real IoT prototype, including hardware control, sensor data, automation logic, dashboard control, and OLED display.

## Demo Goal

The demo should show:

* ESP32 running as a local WebServer.
* Python dashboard connected to ESP32.
* Light control through relay.
* Fan/motor output control through relay.
* Auto Light using PIR motion sensor.
* Auto Fan using DHT11 temperature data.
* Manual / Auto mode switching.
* OLED displaying local device status.
* Real-time synchronization between ESP32 and Dashboard.

## Before Recording

Check these items before starting the demo:

```text
ESP32 is powered on
ESP32 is connected to Wi-Fi
Python dashboard shows "ESP32 Connected"
OLED displays current system status
DHT11 returns real temperature and humidity data
Light relay works
Fan relay works
PIR sensor works
Auto Light mode works
Auto Fan mode works
```

## Suggested Demo Flow

### 1. Introduction

Briefly introduce the project:

```text
This is my Smart Home IoT System built with ESP32, Python Dashboard, sensors, relay module, and OLED display.
```

Show the hardware prototype.

### 2. Show Dashboard Connection

Show the Python dashboard.

Point out:

```text
ESP32 Connected
Temperature
Humidity
Light status
Fan status
Auto / Manual mode buttons
```

### 3. Manual Light Control

Demo steps:

```text
Click Turn ON Light
→ Light turns ON
→ Dashboard updates Light status
→ OLED updates Light status

Click Turn OFF Light
→ Light turns OFF
→ Dashboard updates Light status
→ OLED updates Light status
```

### 4. Auto Light Mode

Demo steps:

```text
Switch Light mode to Auto
Move in front of PIR sensor
→ Light turns ON automatically

Wait for the delay period after no motion
→ Light turns OFF automatically
```

Explain:

```text
The PIR sensor detects motion and ESP32 controls the light automatically.
```

### 5. Manual Fan Control

Demo steps:

```text
Click Turn ON Fan
→ Fan/motor output turns ON
→ Dashboard updates Fan status
→ OLED updates Fan status

Click Turn OFF Fan
→ Fan/motor output turns OFF
→ Dashboard updates Fan status
→ OLED updates Fan status
```

### 6. Auto Fan Mode

Demo steps:

```text
Switch Fan mode to Auto
If temperature is above threshold
→ Fan turns ON automatically
```

Explain:

```text
Auto Fan uses DHT11 temperature data.
When temperature is high, ESP32 turns the fan on automatically.
```

Current threshold:

```text
Fan ON  : Temperature >= 31°C
Fan OFF : Temperature <= 29°C
```

### 7. OLED Display

Show OLED close-up.

OLED should show:

```text
SMART HOME

Temp : xx.x C
Hum  : xx.x %
Light: ON/OFF | Auto/Manual
Fan  : ON/OFF | Auto/Manual
```

Explain:

```text
The OLED provides local status directly on the device, even without looking at the computer dashboard.
```

### 8. API / Status Demo

Optional step:

Open browser:

```text
http://YOUR_ESP32_IP/status
```

Show JSON response:

```json
{
  "light": true,
  "fan": false,
  "autoLight": true,
  "autoFan": false,
  "temp": 32.8,
  "humidity": 68.0
}
```

Explain:

```text
The dashboard reads this JSON API to synchronize real-time device status.
```

## Recommended Video Length

Recommended length:

```text
1 to 2 minutes
```

Suggested structure:

```text
0:00 - 0:10  Project introduction
0:10 - 0:25  Hardware overview
0:25 - 0:45  Dashboard control
0:45 - 1:10  Auto Light and Auto Fan
1:10 - 1:25  OLED display
1:25 - 1:40  API / final overview
```

## Recording Tips

* Use good lighting.
* Keep the hardware visible.
* Make sure Dashboard text is readable.
* Avoid moving the camera too fast.
* Show both hardware and dashboard when possible.
* Keep the demo short and focused.
* If the fan hardware is still a prototype, describe it as fan/motor output or fan simulation.

## Notes for Portfolio

In the portfolio or README, the demo can be described as:

```text
Demo video showing ESP32-based Smart Home IoT prototype with Python dashboard, PIR Auto Light, DHT11 Auto Fan, OLED display, and JSON API synchronization.
```

## Current Prototype Status

The current version is a breadboard prototype.

Completed:

```text
ESP32 WebServer
Python Dashboard
Relay Light Control
Relay Fan/Motor Output Control
PIR Auto Light
DHT11 Temperature and Humidity Monitoring
Auto Fan Mode
Manual / Auto Mode
OLED Local Display
JSON Status API
```

To improve:

```text
More stable fan/motor physical connection
Cleaner wiring layout
Mobile web interface
Static IP or DHCP reservation
Physical enclosure
```
