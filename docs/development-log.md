# Development Log

This document records the development process of the Smart Home IoT System.

---

## Day 1 - ESP32 Wi-Fi and Relay Control

### Goals

The goal of Day 1 was to create the first connection between the ESP32 and a Python application.

Main goals:

* Connect ESP32 to Wi-Fi.
* Create a basic ESP32 WebServer.
* Control a relay from a Python program using HTTP requests.
* Understand the basic flow of an IoT control system.

### Work Completed

On Day 1, the ESP32 was connected to the local Wi-Fi network and assigned an IP address by the router.

A basic WebServer was created on the ESP32. The Python program sent HTTP requests to the ESP32 to control Relay K1.

Main endpoint:

```text
/light_on
/light_off
```

Relay K1 was connected to GPIO23.

### Key Concepts Learned

* ESP32 Wi-Fi connection
* ESP32 WebServer
* Local IP address
* HTTP request
* Python `requests`
* Relay control using GPIO
* Active LOW relay behavior
* Basic client-server communication

### Issues Faced

Some common issues appeared during setup:

* Upload errors in Arduino IDE
* Wrong COM port
* ESP32 not responding
* IP address not found
* Python request timeout
* Undeclared variable errors in code

### Result

By the end of Day 1, the Python application could send commands to the ESP32 and control a relay over Wi-Fi.

---

## Day 2 - LED Control Through Relay

### Goals

The goal of Day 2 was to connect a real output device to the relay.

Main goals:

* Connect an LED through Relay K1.
* Understand how relay COM, NO, and NC terminals work.
* Learn basic breadboard wiring.
* Test relay control with a visible output.

### Work Completed

An LED was connected through Relay K1.

The relay acted as an electronic switch. When the ESP32 controlled the relay, the LED turned ON or OFF.

### Key Concepts Learned

* Relay as a switch
* COM / NO / NC terminals
* LED polarity
* Resistor usage
* Breadboard power rails
* Avoiding VIN-GND short circuits
* Hardware debugging

### Issues Faced

The main challenge was understanding how current flows through the relay and the LED.

There was also a need to be careful with breadboard wiring to avoid short circuits.

### Result

By the end of Day 2, the ESP32 could control a real LED through the relay module.

---

## Day 3 - Fan/Motor Control

### Goals

The goal of Day 3 was to expand the system from one output device to two output devices.

Main goals:

* Add Relay K2.
* Use Relay K2 to control a DC motor as a fan simulation.
* Add fan control API endpoints.
* Understand how to scale the system to multiple devices.

### Work Completed

Relay K2 was connected to GPIO22.

A small DC motor was used as a fan simulation. New endpoints were added:

```text
/fan_on
/fan_off
```

The system could now control:

```text
Relay K1 → Light
Relay K2 → Fan/Motor
```

### Key Concepts Learned

* Multi-device control
* Relay channel expansion
* DC motor polarity
* Separating light and fan logic
* Reusing the same API pattern for new devices

### Issues Faced

The motor connection needed careful checking because loose wires could cause unstable behavior.

### Result

By the end of Day 3, the Smart Home system could control both a light and a fan/motor output.

---

## Day 4 - Status API and Dashboard

### Goals

The goal of Day 4 was to make the system state visible to the user.

Main goals:

* Add device state variables.
* Create a `/status` API endpoint.
* Return system status as JSON.
* Build a Python Tkinter dashboard.
* Automatically sync the dashboard with the ESP32.

### Work Completed

State variables were added:

```cpp
lightState
fanState
```

A `/status` endpoint was created to return JSON data.

Example response:

```json
{
  "light": false,
  "fan": false
}
```

The Python dashboard used `response.json()` to read the ESP32 status.

The dashboard was updated automatically using `app.after()`.

### Key Concepts Learned

* JSON API
* Client-server synchronization
* State management
* REST-like endpoints
* Auto refresh in Tkinter
* Canvas status indicators
* Immediate UI updates

### Issues Faced

The main challenge was keeping the Python dashboard and ESP32 state synchronized.

### Result

By the end of Day 4, the system had a working Python Dashboard that could control and monitor the light and fan states.

---

## Day 5 - PIR, DHT11, and Dashboard UI

### Goals

The goal of Day 5 was to add sensors and automatic behavior.

Main goals:

* Add PIR HC-SR501 motion sensor.
* Add Auto Light mode.
* Add DHT11 temperature and humidity sensor.
* Extend `/status` with sensor data.
* Improve the Python dashboard UI.

### Work Completed

The PIR sensor was connected:

```text
PIR VCC → ESP32 VIN
PIR GND → ESP32 GND
PIR OUT → ESP32 GPIO21
```

The PIR sensor was used to automatically turn on the light when motion was detected.

Instead of using `delay()`, the system used `millis()` to turn the light off after a period of no motion.

Main variables:

```cpp
PIR_PIN = 21
lastMotionTime = 0
lightDelay = 10000
autoLight = true
```

The DHT11 sensor was connected:

```text
DHT11 DATA → ESP32 GPIO19
```

The ESP32 reads temperature and humidity periodically and stores the values.

The `/status` API was extended with:

```json
{
  "autoLight": true,
  "temp": 32.8,
  "humidity": 68.0
}
```

The Python dashboard was improved with:

* Temperature display
* Celsius and Fahrenheit values
* Humidity display
* Connection status
* Color-coded temperature
* Modern dark card-style UI
* Scroll and resize support

### Key Concepts Learned

* PIR motion detection
* Non-blocking timing using `millis()`
* Avoiding `delay()` in IoT systems
* Sensor data caching
* DHT11 library usage
* UI state visualization
* Product-like dashboard design

### Issues Faced

Important issues included:

* Avoiding immediate light OFF in the PIR `else` branch
* Preventing conflict between manual control and PIR automatic control
* Reducing repeated Serial logs
* Handling DHT11 read failures

### Result

By the end of Day 5, the system had automatic light control using PIR, temperature and humidity monitoring using DHT11, and a much better Python dashboard.

---

## Day 6 - Auto Modes, OLED, and Code Cleanup

### Goals

The goal of Day 6 was to make the system more complete and organized.

Main goals:

* Add Auto/Manual mode APIs.
* Add Auto Fan mode.
* Add OLED local display.
* Clean up the code structure.
* Prepare the project for GitHub documentation.

### Work Completed

Auto/Manual endpoints were added for light and fan modes.

Possible endpoints:

```text
/auto_light_on
/auto_light_off
/auto_fan_on
/auto_fan_off
```

Auto Fan mode was added based on temperature.

The OLED display was connected and used to show local device status directly on the hardware.

OLED wiring:

```text
OLED GND → ESP32 GND
OLED VDD → ESP32 3V3
OLED SDA → ESP32 GPIO26
OLED SCK → ESP32 GPIO27
```

Example OLED layout:

```text
SMART HOME

Temp : 32.8 C
Hum  : 68.0 %
Light: ON  | Auto
Fan  : OFF | Manual
```

The ESP32 code and Python dashboard were cleaned up to make the project easier to maintain.

GitHub documentation was started, including:

* README
* API documentation
* Wiring documentation
* Development log
* Demo images

### Key Concepts Learned

* Auto/Manual mode design
* Manual override logic
* OLED display integration
* I2C communication
* Code organization
* GitHub project documentation
* Portfolio project presentation

### Issues Faced

The main challenge was organizing multiple features without making the code too messy.

### Result

By the end of Day 6, the project became a complete Smart Home mini prototype with dashboard, sensors, relays, automatic modes, OLED display, and GitHub documentation.

Checkpoint:

```text
SmartHome_Day6_CleanCode_OLED_Stable
```

---

## Day 7 - Web Control, Static IP, and UI Improvements

### Goals

The goal of Day 7 was to make the Smart Home system easier to use on different devices, especially phones and tablets.

Main goals:

* Add a Web Control interface served directly by ESP32.
* Allow users to control the system from a browser.
* Configure a static IP for stable local access.
* Add EN/VI language switching.
* Improve the web interface with UI transitions and sensor charts.

---

### ESP32 Web Control

Before Day 7, the ESP32 root route `/` only returned a simple text response.

On Day 7, it was replaced with a mobile-friendly Web Control interface.

The Web Control page can be opened at:

```text
http://192.168.1.88
```

The web interface supports:

* Light ON/OFF control
* Fan ON/OFF control
* Auto/Manual mode switching
* Temperature and humidity display
* ESP32 connection status

This makes the system usable from phones, tablets, and computers connected to the same Wi-Fi network.

---

### Reusing Existing ESP32 API

The Web Control interface reuses the existing ESP32 API endpoints:

```text
/status
/light_on
/light_off
/fan_on
/fan_off
/auto_light_on
/auto_light_off
/auto_fan_on
/auto_fan_off
```

The web page reads `/status` every second and updates the UI automatically.

This means the new web interface was added without breaking the existing Python Dashboard or OLED Display.

---

### Static IP Configuration

Previously, the ESP32 IP address could change after a router restart or power outage.

To make the system more stable, a static IP was configured directly in the ESP32 firmware:

```text
192.168.1.88
```

Main URLs:

```text
Web Control:
http://192.168.1.88

Status API:
http://192.168.1.88/status
```

The Python Dashboard was also updated to use the new static IP:

```python
ESP32_IP = "192.168.1.88"
```

Checkpoint:

```text
SmartHome_Day7_StaticIP_WebControl_Stable
```

---

### Web UI Redesign

The first Web Control version had many separate buttons such as ON, OFF, AUTO, and MANUAL.

It was redesigned to be closer to the Python Dashboard style:

```text
LIGHT
Main Room Light
[Turn ON/OFF Light] [Auto/Manual]

FAN
Smart Fan
[Turn ON/OFF Fan] [Auto/Manual]
```

This made the interface cleaner and easier to use on mobile devices.

---

### EN/VI Language Toggle

A language toggle button was added for English and Vietnamese.

The selected language is saved using browser `localStorage`, so the web page remembers the user's language choice.

This avoids relying on automatic browser translation, which caused unstable text rendering because the web UI updates every second.

Checkpoint:

```text
SmartHome_Day7_WebControl_LanguageToggle_Stable
```

---

### UI Transition Effects

The Web Control interface was improved with smooth UI transitions.

Added effects:

* Light/Fan status dots smoothly change color between red and green.
* Status dots have a glow effect.
* Buttons have a small press animation.
* Button colors transition smoothly when states change.

This makes the Web Control feel more like a real product interface.

Checkpoint:

```text
SmartHome_Day7_WebControl_UITransition_Stable
```

---

### Temperature and Humidity Sparkline Charts

The Environment section was redesigned into two separate sensor cards:

```text
Temperature Card
Humidity Card
```

Each card displays:

* Sensor name
* Current value
* Mini sparkline chart

The charts are drawn using lightweight SVG directly in the web page, without external libraries.

The web page stores recent temperature and humidity values in JavaScript arrays and updates the charts every second.

This makes the dashboard more visual and easier to understand.

---

### Fixing Vietnamese Encoding Issues

When Vietnamese text was added directly to the ESP32 web page, some characters displayed incorrectly.

Examples:

```text
Nhà thông minh → NhÃ thÃ´ng minh
Nhiệt độ → Nhiá»‡t Ä‘á»™
°C → Â°C
```

The issue was related to text encoding in the source file.

To fix it, Unicode escape sequences were used for Vietnamese strings and the degree symbol.

Example:

```javascript
title: "Nh\u00E0 th\u00F4ng minh"
```

The degree symbol was also written as:

```javascript
"\u00B0C"
```

After this change, Vietnamese text displayed correctly.

Checkpoint:

```text
SmartHome_Day7_WebControl_Sparkline_UTF8_Stable
```

---

### Hardware Planning

Hardware cleanup and future indoor use were also discussed.

Important notes:

* Dupont wires are acceptable for low-voltage prototype signals.
* Dupont wires must not be used for high-voltage AC wiring.
* For a more stable version, hardware should be mounted on a base plate.
* Breadboard can still be used during the prototype stage.
* Later versions can use terminal blocks, perfboard, or PCB for better reliability.
* Power and signal wiring should be separated clearly.

A wire stripper was chosen and ordered to prepare for future hardware cleanup.

---

### Results

By the end of Day 7, the project had:

* ESP32 Web Control interface
* Static IP configuration
* Mobile-friendly responsive UI
* EN/VI language toggle
* Saved language preference
* Smooth UI transitions
* Temperature and humidity sparkline charts
* Fixed Vietnamese text encoding
* Python Dashboard still working
* OLED Display still working

The project now has three interfaces:

```text
1. Python Dashboard
2. ESP32 Web Control
3. OLED Display
```

---

### Lessons Learned

Day 7 was lighter than Day 6, but it made the project feel much more like a real product.

Key lessons:

* A good API makes it easy to add new interfaces.
* Web UI is a strong choice for multi-platform control.
* Static IP is important for local IoT systems.
* UI/UX matters when designing for real users.
* Browser translation is not reliable for dynamic web apps.
* `localStorage` is useful for saving user preferences.
* SVG can be used to create lightweight charts on ESP32 web pages.
* Vietnamese text on ESP32 web pages may require Unicode escape sequences to avoid encoding issues.
* Hardware used in real environments needs stronger wiring and better mechanical stability than a temporary prototype.

---

### Next Steps

Possible next improvements:

* Add Last Updated time.
* Add ESP32 uptime.
* Add DHT11 OK/Error status.
* Improve hardware wiring when new wires and tools arrive.
* Mount the circuit on a fixed base.
* Record a new demo video.
* Explore remote control using Blynk or Arduino Cloud after the exam.
