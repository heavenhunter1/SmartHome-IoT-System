# Setup Guide

This document explains how to set up and run the Smart Home IoT System.

The project includes three main parts:

```text
1. ESP32 firmware
2. Python Tkinter dashboard
3. ESP32 Web Control interface
```

---

## 1. Requirements

### Hardware

* ESP32 development board
* 2-channel relay module
* PIR HC-SR501 motion sensor
* DHT11 temperature and humidity sensor
* 0.96 inch I2C OLED display
* LED
* DC motor / fan simulation
* Breadboard
* Dupont wires
* Resistor
* Micro USB cable

### Software

* Arduino IDE
* Python 3
* Required Python package:

  * `requests`

### Arduino Libraries

Install these libraries in Arduino IDE:

* DHT sensor library by Adafruit
* Adafruit Unified Sensor
* Adafruit GFX Library
* Adafruit SSD1306

---

## 2. ESP32 Firmware Setup

Open the ESP32 firmware file:

```text
esp32/SmartHome_ESP32.ino
```

Before uploading the code, update the Wi-Fi configuration:

```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
```

Do not upload real Wi-Fi credentials to a public GitHub repository.

---

## 3. Static IP Configuration

The current firmware uses a static IP configuration for stable local access.

Default local IP:

```text
192.168.1.88
```

Default Web Control URL:

```text
http://192.168.1.88
```

Default Status API URL:

```text
http://192.168.1.88/status
```

If your local network uses a different IP range, update the static IP configuration in the ESP32 firmware:

```cpp
IPAddress local_IP(192, 168, 1, 88);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);
```

---

## 4. ESP32 Board Settings

In Arduino IDE, select:

```text
Board: ESP32 Dev Module
Port : Your ESP32 COM port
```

Then click:

```text
Verify
Upload
```

If upload fails, try:

* Closing Serial Monitor
* Unplugging and reconnecting the ESP32
* Holding the BOOT button while uploading
* Checking the USB cable
* Restarting Arduino IDE

---

## 5. Check ESP32 IP Address

After uploading, open Serial Monitor at:

```text
115200 baud
```

Press the EN / RST button on ESP32.

You should see output similar to:

```text
IP: 192.168.1.88
HTTP Server Started
```

This IP address is used for the Python dashboard and Web Control interface.

---

## 6. Test ESP32 API

Open a browser and visit:

```text
http://192.168.1.88/status
```

Expected response:

```json
{
  "light": false,
  "fan": false,
  "autoLight": true,
  "autoFan": false,
  "temp": 32.8,
  "humidity": 68.0
}
```

---

## 7. Web Control Setup

The ESP32 serves a mobile-friendly Web Control interface directly from the root route `/`.

Open a browser and visit:

```text
http://192.168.1.88
```

The Web Control page can be used on phones, tablets, and computers connected to the same Wi-Fi network.

The Web Control interface supports:

* Light ON/OFF control
* Fan ON/OFF control
* Auto/Manual mode switching
* Temperature and humidity monitoring
* Mini sparkline charts for temperature and humidity trends
* EN/VI language toggle
* Saved language preference using browser localStorage

The Web Control page updates device status automatically using the `/status` API.

---

## 8. Python Dashboard Setup

Open the dashboard file:

```text
dashboard/dashboard.py
```

Update the ESP32 IP address:

```python
ESP32_IP = "192.168.1.88"
```

Install the required Python package:

```bash
pip install requests
```

Run the dashboard:

```bash
python dashboard.py
```

---

## 9. Dashboard Features

The Python dashboard can:

* Turn light ON/OFF
* Turn fan ON/OFF
* Switch Light mode between Auto and Manual
* Switch Fan mode between Auto and Manual
* Display temperature in Celsius and Fahrenheit
* Display humidity
* Show ESP32 connection status
* Automatically refresh status every second

---

## 10. OLED Display

The OLED display shows local device status directly on the hardware prototype.

Example OLED layout:

```text
SMART HOME

Temp : 32.8 C
Hum  : 68.0 %
Light: ON  | Auto
Fan  : OFF | Manual
```

OLED wiring:

```text
OLED GND → ESP32 GND
OLED VDD → ESP32 3V3
OLED SDA → ESP32 GPIO26
OLED SCK → ESP32 GPIO27
```

---

## 11. Common Issues

### Dashboard or Web Control shows ESP32 Disconnected

Possible causes:

* ESP32 is not powered
* ESP32 and the device are not on the same Wi-Fi network
* Wi-Fi credentials are incorrect
* Static IP configuration does not match your local network
* ESP32 firmware is not running

Fix:

```text
1. Open Serial Monitor
2. Check the current ESP32 IP address
3. Open http://192.168.1.88/status in a browser
4. Update ESP32_IP in dashboard.py if needed
5. Run the dashboard again
```

### Web Control does not open

Possible causes:

* The device is not connected to the same Wi-Fi network
* ESP32 is not powered
* The static IP is different from your network range
* Browser cache is loading an old page

Try:

```text
Refresh the page
Open a new tab
Check Serial Monitor
Visit http://192.168.1.88/status
```

### DHT11 does not return data

Possible causes:

* Wrong wiring
* Missing DHT library
* Sensor is read too frequently
* Loose Dupont wires

### OLED does not display

Possible causes:

* Wrong SDA/SCK wiring
* Wrong I2C address
* Missing Adafruit SSD1306 or GFX library
* Loose wires

### ESP32 upload fails

Possible causes:

* Serial Monitor is open
* Wrong COM port
* USB cable only supports charging
* ESP32 did not enter boot mode

Try:

```text
Unplug ESP32
Plug it back in
Select the correct port
Hold BOOT while uploading if needed
```

---

## 12. Notes

* The current firmware uses static IP `192.168.1.88` for stable local access.
* If your local network uses a different IP range, update the static IP configuration in the ESP32 firmware.
* Router DHCP reservation can also be used as an alternative long-term solution.
* Do not upload real Wi-Fi credentials to GitHub.
* The current hardware is a breadboard prototype.
* A more stable motor/fan connection can be added in a future hardware revision.
* Do not use Dupont wires for high-voltage AC wiring.
