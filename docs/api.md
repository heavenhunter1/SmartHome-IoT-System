# API Documentation

This document describes the HTTP API endpoints provided by the ESP32 WebServer in the Smart Home IoT System.

The Python dashboard communicates with the ESP32 using HTTP requests over the local Wi-Fi network.

## Base URL

```text
http://YOUR_ESP32_IP
```

Example:

```text
http://192.168.1.8
```

> Note: The ESP32 IP address may change if the router restarts or DHCP assigns a new address.

## Status Endpoint

### `GET /status`

Returns the current system status as JSON.

### Example Request

```text
GET http://YOUR_ESP32_IP/status
```

### Example Response

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

### Response Fields

| Field       | Type    | Description            |
| ----------- | ------- | ---------------------- |
| `light`     | boolean | Current light state    |
| `fan`       | boolean | Current fan state      |
| `autoLight` | boolean | Auto Light mode state  |
| `autoFan`   | boolean | Auto Fan mode state    |
| `temp`      | number  | Temperature in Celsius |
| `humidity`  | number  | Humidity percentage    |

## Light Control

### `GET /light_on`

Turns the light ON.

```text
GET http://YOUR_ESP32_IP/light_on
```

Response:

```text
Light ON
```

### `GET /light_off`

Turns the light OFF.

```text
GET http://YOUR_ESP32_IP/light_off
```

Response:

```text
Light OFF
```

## Fan Control

### `GET /fan_on`

Turns the fan or motor output ON.

```text
GET http://YOUR_ESP32_IP/fan_on
```

Response:

```text
Fan ON
```

### `GET /fan_off`

Turns the fan or motor output OFF.

```text
GET http://YOUR_ESP32_IP/fan_off
```

Response:

```text
Fan OFF
```

## Auto Light Mode

### `GET /auto_light_on`

Enables Auto Light mode.

In Auto Light mode, the PIR sensor can automatically turn the light ON or OFF based on motion detection.

```text
GET http://YOUR_ESP32_IP/auto_light_on
```

Response:

```text
Auto Light ON
```

### `GET /auto_light_off`

Disables Auto Light mode.

In Manual mode, the light is controlled only by the dashboard.

```text
GET http://YOUR_ESP32_IP/auto_light_off
```

Response:

```text
Auto Light OFF
```

## Auto Fan Mode

### `GET /auto_fan_on`

Enables Auto Fan mode.

In Auto Fan mode, the ESP32 automatically controls the fan based on temperature.

```text
GET http://YOUR_ESP32_IP/auto_fan_on
```

Response:

```text
Auto Fan ON
```

### `GET /auto_fan_off`

Disables Auto Fan mode.

In Manual mode, the fan is controlled only by the dashboard.

```text
GET http://YOUR_ESP32_IP/auto_fan_off
```

Response:

```text
Auto Fan OFF
```

## Automation Logic

### Auto Light

```text
PIR detects motion
→ Light turns ON

No motion for a delay period
→ Light turns OFF
```

### Auto Fan

```text
Temperature >= 31°C
→ Fan turns ON

Temperature <= 29°C
→ Fan turns OFF
```

The system uses a hysteresis range to prevent the fan from switching ON and OFF too frequently.

## Manual Override

When Auto Fan mode is enabled, the user can still manually control the fan.

If the user manually turns the fan ON or OFF, Auto Fan is temporarily paused for a period of time before resuming.

This prevents the automation system from immediately overriding the user's command.

## Dashboard Communication Flow

```text
Python Dashboard
      ↓
HTTP Request
      ↓
ESP32 WebServer
      ↓
Relay / Sensor / State Update
      ↓
JSON Status Response
      ↓
Dashboard UI Update
```

## Notes

* All API endpoints are local network endpoints.
* The ESP32 and dashboard device must be connected to the same Wi-Fi network.
* The ESP32 IP address may change after router restart unless static IP or DHCP reservation is configured.
* Wi-Fi credentials should not be uploaded to public repositories.
