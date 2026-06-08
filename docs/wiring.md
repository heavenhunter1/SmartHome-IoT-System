# Wiring Guide

This document describes the hardware wiring used in the Smart Home IoT System.

## ESP32 Pin Mapping

| Component             | ESP32 Pin | Description                     |
| --------------------- | --------: | ------------------------------- |
| Relay K1 - Light      |    GPIO23 | Controls LED / light output     |
| Relay K2 - Fan        |    GPIO22 | Controls fan / motor output     |
| PIR HC-SR501          |    GPIO21 | Motion detection input          |
| DHT11                 |    GPIO19 | Temperature and humidity sensor |
| OLED SDA              |    GPIO26 | I2C data line                   |
| OLED SCK / SCL        |    GPIO27 | I2C clock line                  |
| Built-in / status LED |     GPIO2 | LED status indicator            |

## Relay Module

The relay module is used to control external devices such as the light and fan/motor output.

The relay module used in this project is active LOW:

```text
LOW  → Relay ON
HIGH → Relay OFF
```

## Relay K1 - Light

Relay K1 is used to control the light output.

```text
ESP32 GPIO23 → Relay IN1
Relay K1     → LED / Light circuit
```

Logic:

```text
Light ON
→ Relay K1 turns ON
→ LED / light turns ON

Light OFF
→ Relay K1 turns OFF
→ LED / light turns OFF
```

## Relay K2 - Fan / Motor Output

Relay K2 is used for fan or motor output control.

```text
ESP32 GPIO22 → Relay IN2
Relay K2     → Fan / Motor output
```

At the current prototype stage, the fan output is tested using an RF-310T DC motor / fan simulation.

A more stable physical fan connection will be added in a future hardware revision.

## PIR HC-SR501 Motion Sensor

The PIR sensor is used for Auto Light mode.

```text
PIR VCC → ESP32 VIN
PIR GND → ESP32 GND
PIR OUT → ESP32 GPIO21
```

PIR output logic:

```text
Motion detected → HIGH
No motion       → LOW
```

System behavior:

```text
Motion detected
→ Light turns ON

No motion for a delay period
→ Light turns OFF
```

The system uses `millis()` instead of `delay()` so that the ESP32 can continue handling web requests while waiting.

## DHT11 Sensor

The DHT11 sensor is used to read temperature and humidity.

```text
DHT11 VCC / +  → ESP32 VIN
DHT11 GND / -  → ESP32 GND
DHT11 DATA / S → ESP32 GPIO19
```

The ESP32 reads DHT11 data periodically instead of reading it on every dashboard request.

This helps keep the system stable and avoids reading the sensor too frequently.

## OLED Display

The OLED display uses I2C communication.

```text
OLED GND → ESP32 GND
OLED VDD → ESP32 3V3
OLED SDA → ESP32 GPIO26
OLED SCK → ESP32 GPIO27
```

The OLED displays local device status:

```text
SMART HOME

Temp : xx.x C
Hum  : xx.x %
Light: ON/OFF | Auto/Manual
Fan  : ON/OFF | Auto/Manual
```

## Power Notes

* ESP32 and all modules must share a common GND.
* OLED is powered from 3V3.
* PIR and DHT11 are powered according to their module requirements.
* Relay module is powered according to its module requirement.
* Avoid directly shorting VIN and GND through the relay.
* External motors or fans may require a separate power supply depending on voltage and current requirements.
* A relay works as a switch; it does not generate power by itself.

## Breadboard Prototype Notes

This project is currently built as a breadboard prototype.

Because it is a prototype, some wiring may be temporary and can be improved in future versions.

Current prototype status:

```text
Completed:
- ESP32 WebServer
- Relay K1 light control
- Relay K2 fan/motor output logic
- PIR Auto Light
- DHT11 temperature and humidity monitoring
- OLED local display
- Python dashboard control

To improve:
- More stable physical fan/motor connection
- Cleaner wiring layout
- Possible enclosure or mounted board
```

## Safety Notes

* Always check wiring before powering the circuit.
* Do not connect VIN directly to GND.
* Make sure relay COM / NO / NC connections are correct.
* Check LED polarity before testing.
* Use a suitable power supply for motors or fans.
* If the motor or fan requires higher current, use a separate power supply and ensure common GND when needed.

## Current Prototype Note

Relay K2 is designed for fan control.

At the current prototype stage, the fan logic and relay output are implemented. The physical motor/fan connection is still being improved for a more stable hardware demo.

This project focuses on understanding the full IoT system flow:

```text
Sensors
→ ESP32
→ JSON API
→ Dashboard
→ Automation Logic
→ Local OLED Display
```
