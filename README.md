# SmartHome-IoT-System

An ESP32-based Smart Home IoT prototype with a Python Tkinter dashboard, sensors, automation logic, and OLED display.

This project was built as a personal hands-on learning project to understand how IoT systems work, including hardware control, local networking, JSON APIs, automation logic, and user interface design.

## Features

* Control light using ESP32 and relay.
* Control fan/motor using ESP32 and relay.
* Python Tkinter dashboard for local Wi-Fi control.
* Real-time device status synchronization using JSON API.
* PIR motion-based Auto Light mode.
* DHT11 temperature and humidity monitoring.
* Auto Fan mode based on temperature.
* Manual / Auto mode for both light and fan.
* Manual override logic for Auto Fan.
* OLED display for local device status.
* Dark dashboard UI with scroll and resize support.
* Clean and organized ESP32 firmware structure.
* Clean and organized Python dashboard structure.

## Hardware Components

* ESP32 development board
* 2-channel relay module
* PIR HC-SR501 motion sensor
* DHT11 temperature and humidity sensor
* 0.96 inch I2C OLED display
* LED
* RF-310T DC motor
* Breadboard
* Dupont wires
* Resistor

## Technologies Used

* ESP32
* Arduino IDE
* Python
* Tkinter
* HTTP Request
* ESP32 WebServer
* JSON API
* I2C OLED Display

## System Architecture

```text
Sensors
(PIR HC-SR501, DHT11)
        ↓
ESP32 WebServer
        ↓
JSON API
        ↓
Python Tkinter Dashboard

ESP32 also controls:
- Relay K1 → Light / LED
- Relay K2 → Fan / DC motor
- OLED Display → Local device status
```

## Main Functions

### Light Control

The light can be controlled manually from the Python dashboard.

The system also supports Auto Light mode using the PIR motion sensor.

```text
Motion detected
→ Light turns ON

No motion for a delay period
→ Light turns OFF
```

### Fan Control

The fan can be controlled manually from the Python dashboard.

The system also supports Auto Fan mode based on temperature.

```text
Temperature >= 31°C
→ Fan turns ON

Temperature <= 29°C
→ Fan turns OFF
```

A hysteresis range is used to prevent the fan from turning on and off too frequently.

### Manual / Auto Mode

Both light and fan support Manual / Auto mode.

```text
Manual Mode
→ User controls the device from the dashboard

Auto Mode
→ ESP32 controls the device based on sensor data
```

### Manual Override

When Auto Fan is enabled, the user can still manually turn the fan ON or OFF.
After manual control, Auto Fan is temporarily paused for a period of time before resuming.

This prevents the system from immediately overriding the user's command.

## API Endpoints

| Endpoint          | Description                  |
| ----------------- | ---------------------------- |
| `/`               | Check if ESP32 is online     |
| `/status`         | Return system status as JSON |
| `/light_on`       | Turn light on                |
| `/light_off`      | Turn light off               |
| `/fan_on`         | Turn fan on                  |
| `/fan_off`        | Turn fan off                 |
| `/auto_light_on`  | Enable Auto Light mode       |
| `/auto_light_off` | Disable Auto Light mode      |
| `/auto_fan_on`    | Enable Auto Fan mode         |
| `/auto_fan_off`   | Disable Auto Fan mode        |

Example `/status` response:

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

## OLED Display

The OLED display shows local device status directly on the Smart Home prototype.

Example OLED layout:

```text
SMART HOME

Temp : 32.8 C
Hum  : 68.0 %
Light: ON  | Auto
Fan  : OFF | Manual
```

## Dashboard

The Python Tkinter dashboard allows users to:

* Turn light ON/OFF.
* Turn fan ON/OFF.
* Switch Light mode between Auto and Manual.
* Switch Fan mode between Auto and Manual.
* View temperature and humidity.
* View ESP32 connection status.
* Monitor device states in real time.

The dashboard uses HTTP requests to communicate with ESP32 and automatically refreshes system status.

## What I Learned

Through this project, I learned:

* How ESP32 connects to Wi-Fi and works as a local web server.
* How to control relays using GPIO pins.
* How to use HTTP requests to control hardware.
* How to design JSON APIs for IoT devices.
* How to synchronize device state between ESP32 and a Python dashboard.
* How to use PIR and DHT11 sensors in an automation system.
* How to use an OLED display with custom I2C pins.
* How to avoid blocking code by using `millis()` instead of `delay()`.
* How to handle conflicts between manual control and automatic control.
* How to design Manual / Auto modes for IoT devices.
* How to debug hardware, software, upload, library, and network issues.
* How to organize code into cleaner functions for better maintainability.

## Project Structure

```text
SmartHome-IoT-System/
│
├── esp32/
│   └── SmartHome_ESP32.ino
│
├── dashboard/
│   └── dashboard.py
│
├── docs/
│   └── development-log.md
│
├── images/
│   ├── circuit.jpg
│   ├── dashboard.png
│   └── oled.jpg
│
└── README.md
```

## Security Note

Wi-Fi credentials are not included in this repository.

Before uploading code publicly, replace real Wi-Fi credentials with placeholders:

```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
```

For a more secure setup, Wi-Fi credentials can be stored in a separate local configuration file that is not uploaded to GitHub.

## Future Improvements

* Add a mobile web interface.
* Add static IP or router DHCP reservation.
* Improve dashboard UI with a more modern framework.
* Add temperature and humidity data logging.
* Add more sensors such as gas, light, or door sensors.
* Build a physical enclosure for the system.
* Add user settings for temperature thresholds.
* Deploy the project as a complete portfolio demo.
