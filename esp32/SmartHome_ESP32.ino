#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// =====================================================
// WIFI CONFIG
// =====================================================
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
// Static IP configuration
// Change these values to match your local network.
IPAddress local_IP(192, 168, 1, 88);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

// =====================================================
// PIN CONFIG
// =====================================================
const int LED_PIN = 2;

const int LIGHT_RELAY_PIN = 23;  // Relay K1 - Light
const int FAN_RELAY_PIN = 22;    // Relay K2 - Fan

const int PIR_PIN = 21;
const int DHT_PIN = 19;

const int OLED_SDA_PIN = 26;
const int OLED_SCL_PIN = 27;

// =====================================================
// OLED CONFIG
// =====================================================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

bool oledReady = false;

// =====================================================
// DHT11 CONFIG
// =====================================================
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);

float temp = 0;
float humidity = 0;

unsigned long lastDHTReadTime = 0;
const unsigned long dhtReadDelay = 3000;

// =====================================================
// LIGHT STATE / AUTO LIGHT
// =====================================================
bool lightState = false;
bool autoLight = true;
bool lightControlledByPIR = false;

unsigned long lastMotionTime = 0;
const unsigned long lightDelay = 10000;

int lastMotionState = LOW;

// =====================================================
// FAN STATE / AUTO FAN
// =====================================================
bool fanState = false;
bool autoFan = false;

bool fanOverride = false;
unsigned long fanOverrideStartTime = 0;
const unsigned long fanOverrideDelay = 60000;

const float fanOnTemp = 31.0;
const float fanOffTemp = 29.0;

// =====================================================
// SERVER
// =====================================================
WebServer server(80);

const char MAIN_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">

  <title>Smart Home</title>

  <style>
    body {
      margin: 0;
      font-family: Arial, sans-serif;
      background: #0f172a;
      color: #e5e7eb;
    }

    .container {
      max-width: 420px;
      margin: auto;
      padding: 20px;
    }

    h1 {
      margin-bottom: 4px;
      font-size: 28px;
    }

    .subtitle {
      color: #94a3b8;
      margin-bottom: 14px;
    }

    .header-row {
      display: flex;
      justify-content: space-between;
      align-items: center;
      gap: 12px;
    }

    .small-btn {
      width: auto;
      padding: 8px 12px;
      border: none;
      border-radius: 10px;
      background: #334155;
      color: white;
      font-weight: bold;
      font-size: 13px;
    }

    .connection {
      font-weight: bold;
      color: #eab308;
      margin-bottom: 18px;
    }

    .sensor-grid {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 12px;
      margin-bottom: 14px;
    }

    .sensor-card {
      background: #1e293b;
      border-radius: 16px;
      padding: 14px;
      min-height: 145px;
      box-shadow: inset 0 0 0 1px rgba(148, 163, 184, 0.08);
    }

    .sensor-title {
      color: #94a3b8;
      font-size: 13px;
      font-weight: bold;
      margin-bottom: 10px;
    }

    .sensor-value {
      font-size: 26px;
      font-weight: bold;
      margin-bottom: 10px;
    }

    .sparkline {
      width: 100%;
      height: 60px;
      overflow: visible;
    }

    .grid-line {
      stroke: rgba(148, 163, 184, 0.16);
      stroke-width: 1;
    }

    .temp-line,
    .humidity-line {
      fill: none;
      stroke: #38bdf8;
      stroke-width: 3;
      stroke-linecap: round;
      stroke-linejoin: round;
      transition: all 0.4s ease;
    }

    .temp-area,
    .humidity-area {
      fill: rgba(56, 189, 248, 0.16);
      transition: all 0.4s ease;
    }

    .card {
      background: #1e293b;
      border-radius: 16px;
      padding: 16px;
      margin-bottom: 14px;
    }

    .card-top {
      display: flex;
      justify-content: space-between;
      align-items: center;
      margin-bottom: 14px;
    }

    .card-title {
      color: #94a3b8;
      font-size: 13px;
      font-weight: bold;
      letter-spacing: 1px;
    }

    .state {
      font-size: 13px;
      font-weight: bold;
    }

    .device-row {
      display: flex;
      align-items: center;
      margin-bottom: 14px;
    }

    .dot {
      width: 16px;
      height: 16px;
      border-radius: 4px;
      background: #ef4444;
      margin-right: 12px;
      box-shadow: 0 0 12px rgba(239, 68, 68, 0.7);
      transition: background-color 0.4s ease, box-shadow 0.4s ease;
    }

    .device-name {
      font-size: 20px;
      font-weight: bold;
    }

    .button-row {
      display: flex;
      gap: 8px;
    }

    button {
      border: none;
      border-radius: 10px;
      padding: 12px;
      color: white;
      font-weight: bold;
      font-size: 14px;
      transition: background-color 0.4s ease, transform 0.15s ease, box-shadow 0.4s ease;
    }

    button:active {
      transform: scale(0.97);
    }

    .main-btn {
      flex: 1;
      background: #2563eb;
    }

    .mode-btn {
      width: 90px;
      background: #22c55e;
    }

    .blue {
      color: #3b82f6;
    }

    .green {
      color: #22c55e;
    }

    .red {
      color: #ef4444;
    }

    .yellow {
      color: #eab308;
    }

    .gray-btn {
      background: #475569;
    }

    .yellow-btn {
      background: #eab308;
    }

    .green-btn {
      background: #22c55e;
    }
  </style>
</head>

<body>
  <div class="container">

    <div class="header-row">
      <div>
        <h1 id="titleText">Smart Home</h1>
        <div id="subtitleText" class="subtitle">ESP32 Web Control</div>
      </div>

      <button class="small-btn" onclick="toggleLanguage()" id="langBtn">VI</button>
    </div>

    <div id="connection" class="connection">Connecting...</div>

    <div class="sensor-grid">
      <div class="sensor-card">
        <div id="temperatureLabel" class="sensor-title">Temperature</div>
        <div id="temp" class="sensor-value">--.- Â°C</div>

        <svg class="sparkline" viewBox="0 0 140 60">
          <line x1="0" y1="15" x2="140" y2="15" class="grid-line" />
          <line x1="0" y1="30" x2="140" y2="30" class="grid-line" />
          <line x1="0" y1="45" x2="140" y2="45" class="grid-line" />

          <polygon id="tempArea" points="" class="temp-area"></polygon>
          <polyline id="tempChart" points="" class="temp-line"></polyline>
        </svg>
      </div>

      <div class="sensor-card">
        <div id="humidityLabel" class="sensor-title">Humidity</div>
        <div id="humidity" class="sensor-value blue">--.- %</div>

        <svg class="sparkline" viewBox="0 0 140 60">
          <line x1="0" y1="15" x2="140" y2="15" class="grid-line" />
          <line x1="0" y1="30" x2="140" y2="30" class="grid-line" />
          <line x1="0" y1="45" x2="140" y2="45" class="grid-line" />

          <polygon id="humidityArea" points="" class="humidity-area"></polygon>
          <polyline id="humidityChart" points="" class="humidity-line"></polyline>
        </svg>
      </div>
    </div>

    <div class="card">
      <div class="card-top">
        <div id="lightTitle" class="card-title">LIGHT</div>
        <div id="lightStateText" class="state red">OFF</div>
      </div>

      <div class="device-row">
        <div id="lightDot" class="dot"></div>
        <div id="lightDeviceName" class="device-name">Main Room Light</div>
      </div>

      <div class="button-row">
        <button id="lightBtn" class="main-btn" onclick="toggleLight()">Turn ON Light</button>
        <button id="autoLightBtn" class="mode-btn" onclick="toggleAutoLight()">Auto</button>
      </div>
    </div>

    <div class="card">
      <div class="card-top">
        <div id="fanTitle" class="card-title">FAN</div>
        <div id="fanStateText" class="state red">OFF</div>
      </div>

      <div class="device-row">
        <div id="fanDot" class="dot"></div>
        <div id="fanDeviceName" class="device-name">Smart Fan</div>
      </div>

      <div class="button-row">
        <button id="fanBtn" class="main-btn" onclick="toggleFan()">Turn ON Fan</button>
        <button id="autoFanBtn" class="mode-btn yellow-btn" onclick="toggleAutoFan()">Manual</button>
      </div>
    </div>
  </div>

  <script>
    let lang = localStorage.getItem("smartHomeLang") || "en";

    const texts = {
      en: {
        title: "Smart Home",
        subtitle: "ESP32 Web Control",
        connecting: "Connecting...",
        connected: "ESP32 Connected",
        disconnected: "ESP32 Disconnected",

        temperature: "Temperature",
        humidity: "Humidity",

        light: "LIGHT",
        fan: "FAN",
        mainRoomLight: "Main Room Light",
        smartFan: "Smart Fan",

        on: "ON",
        off: "OFF",
        auto: "Auto",
        manual: "Manual",

        turnOnLight: "Turn ON Light",
        turnOffLight: "Turn OFF Light",
        turnOnFan: "Turn ON Fan",
        turnOffFan: "Turn OFF Fan",

        language: "VI"
      },

      vi: {
  title: "Nh\u00E0 th\u00F4ng minh",
  subtitle: "\u0110i\u1EC1u khi\u1EC3n ESP32",
  connecting: "\u0110ang k\u1EBFt n\u1ED1i...",
  connected: "ESP32 \u0111\u00E3 k\u1EBFt n\u1ED1i",
  disconnected: "ESP32 m\u1EA5t k\u1EBFt n\u1ED1i",

  environment: "M\u00D4I TR\u01AF\u1EDCNG",
  temperature: "Nhi\u1EC7t \u0111\u1ED9",
  humidity: "\u0110\u1ED9 \u1EA9m",

  light: "\u0110\u00C8N",
  fan: "QU\u1EA0T",
  mainRoomLight: "\u0110\u00E8n ph\u00F2ng ch\u00EDnh",
  smartFan: "Qu\u1EA1t th\u00F4ng minh",

  on: "B\u1EACT",
  off: "T\u1EAET",
  auto: "T\u1EF1 \u0111\u1ED9ng",
  manual: "Th\u1EE7 c\u00F4ng",

  turnOnLight: "B\u1EADt \u0111\u00E8n",
  turnOffLight: "T\u1EAFt \u0111\u00E8n",
  turnOnFan: "B\u1EADt qu\u1EA1t",
  turnOffFan: "T\u1EAFt qu\u1EA1t",

  language: "EN"
}
    };

    let lightState = false;
    let fanState = false;
    let autoLight = true;
    let autoFan = false;

    let tempHistory = [];
    let humidityHistory = [];
    const maxHistoryPoints = 14;

    function toggleLanguage() {
      lang = lang === "en" ? "vi" : "en";
      localStorage.setItem("smartHomeLang", lang);

      applyLanguage();
      updateLightUI();
      updateFanUI();
    }

    function applyLanguage() {
      const t = texts[lang];

      document.getElementById("titleText").innerText = t.title;
      document.getElementById("subtitleText").innerText = t.subtitle;
      document.getElementById("langBtn").innerText = t.language;

      document.getElementById("temperatureLabel").innerText = t.temperature;
      document.getElementById("humidityLabel").innerText = t.humidity;

      document.getElementById("lightTitle").innerText = t.light;
      document.getElementById("fanTitle").innerText = t.fan;

      document.getElementById("lightDeviceName").innerText = t.mainRoomLight;
      document.getElementById("fanDeviceName").innerText = t.smartFan;
    }

    async function sendCommand(cmd) {
      try {
        await fetch('/' + cmd);
        updateStatus();
      } catch (error) {
        setDisconnected();
      }
    }

    function toggleLight() {
      if (lightState) {
        sendCommand('light_off');
      } else {
        sendCommand('light_on');
      }
    }

    function toggleFan() {
      if (fanState) {
        sendCommand('fan_off');
      } else {
        sendCommand('fan_on');
      }
    }

    function toggleAutoLight() {
      if (autoLight) {
        sendCommand('auto_light_off');
      } else {
        sendCommand('auto_light_on');
      }
    }

    function toggleAutoFan() {
      if (autoFan) {
        sendCommand('auto_fan_off');
      } else {
        sendCommand('auto_fan_on');
      }
    }

    function setDisconnected() {
      const t = texts[lang];

      const connection = document.getElementById('connection');
      connection.innerText = t.disconnected;
      connection.style.color = '#ef4444';
    }

    async function updateStatus() {
      try {
        const response = await fetch('/status');
        const data = await response.json();

        lightState = data.light;
        fanState = data.fan;
        autoLight = data.autoLight;
        autoFan = data.autoFan;

        const connection = document.getElementById('connection');
        connection.innerText = texts[lang].connected;
        connection.style.color = '#22c55e';

        updateEnvironment(data.temp, data.humidity);
        updateLightUI();
        updateFanUI();

      } catch (error) {
        setDisconnected();
      }
    }

    function addHistoryValue(history, value) {
      history.push(value);

      if (history.length > maxHistoryPoints) {
        history.shift();
      }
    }

    function drawSparkline(lineId, areaId, history, minValue, maxValue) {
      const line = document.getElementById(lineId);
      const area = document.getElementById(areaId);

      if (history.length < 2) {
        line.setAttribute("points", "");
        area.setAttribute("points", "");
        return;
      }

      const width = 140;
      const height = 60;
      const bottom = 58;
      const stepX = width / (maxHistoryPoints - 1);

      let linePoints = "";

      for (let i = 0; i < history.length; i++) {
        let value = history[i];
        let percent = (value - minValue) / (maxValue - minValue);

        if (percent < 0) percent = 0;
        if (percent > 1) percent = 1;

        const x = i * stepX;
        const y = bottom - percent * 46;

        linePoints += x + "," + y + " ";
      }

      const firstX = 0;
      const lastX = (history.length - 1) * stepX;

      const areaPoints =
        firstX + "," + bottom + " " +
        linePoints +
        lastX + "," + bottom;

      line.setAttribute("points", linePoints.trim());
      area.setAttribute("points", areaPoints.trim());
    }

    function updateEnvironment(temp, humidity) {
      const tempElement = document.getElementById('temp');
      const humidityElement = document.getElementById('humidity');

      tempElement.innerText = temp.toFixed(1) + ' °C';
      humidityElement.innerText = humidity.toFixed(1) + ' %';

      tempElement.classList.remove('blue', 'green', 'red');

      if (temp < 22) {
        tempElement.classList.add('blue');
      } else if (temp <= 30) {
        tempElement.classList.add('green');
      } else {
        tempElement.classList.add('red');
      }

      addHistoryValue(tempHistory, temp);
      addHistoryValue(humidityHistory, humidity);

      drawSparkline('tempChart', 'tempArea', tempHistory, 15, 40);
      drawSparkline('humidityChart', 'humidityArea', humidityHistory, 30, 90);
    }

    function updateLightUI() {
      const t = texts[lang];

      const stateText = document.getElementById('lightStateText');
      const dot = document.getElementById('lightDot');
      const btn = document.getElementById('lightBtn');
      const modeBtn = document.getElementById('autoLightBtn');

      if (lightState) {
        stateText.innerText = t.on;
        stateText.className = 'state green';
        dot.style.backgroundColor = '#22c55e';
        dot.style.boxShadow = '0 0 12px rgba(34, 197, 94, 0.7)';
        btn.innerText = t.turnOffLight;
        btn.className = 'main-btn gray-btn';
      } else {
        stateText.innerText = t.off;
        stateText.className = 'state red';
        dot.style.backgroundColor = '#ef4444';
        dot.style.boxShadow = '0 0 12px rgba(239, 68, 68, 0.7)';
        btn.innerText = t.turnOnLight;
        btn.className = 'main-btn';
      }

      if (autoLight) {
        modeBtn.innerText = t.auto;
        modeBtn.className = 'mode-btn green-btn';
      } else {
        modeBtn.innerText = t.manual;
        modeBtn.className = 'mode-btn yellow-btn';
      }
    }

    function updateFanUI() {
      const t = texts[lang];

      const stateText = document.getElementById('fanStateText');
      const dot = document.getElementById('fanDot');
      const btn = document.getElementById('fanBtn');
      const modeBtn = document.getElementById('autoFanBtn');

      if (fanState) {
        stateText.innerText = t.on;
        stateText.className = 'state green';
        dot.style.backgroundColor = '#22c55e';
        dot.style.boxShadow = '0 0 12px rgba(34, 197, 94, 0.7)';
        btn.innerText = t.turnOffFan;
        btn.className = 'main-btn gray-btn';
      } else {
        stateText.innerText = t.off;
        stateText.className = 'state red';
        dot.style.backgroundColor = '#ef4444';
        dot.style.boxShadow = '0 0 12px rgba(239, 68, 68, 0.7)';
        btn.innerText = t.turnOnFan;
        btn.className = 'main-btn';
      }

      if (autoFan) {
        modeBtn.innerText = t.auto;
        modeBtn.className = 'mode-btn green-btn';
      } else {
        modeBtn.innerText = t.manual;
        modeBtn.className = 'mode-btn yellow-btn';
      }
    }

    applyLanguage();
    setInterval(updateStatus, 1000);
    updateStatus();
  </script>
</body>
</html>
)rawliteral";
// =====================================================
// DEVICE CONTROL HELPERS
// =====================================================
void setLight(bool on) {
  digitalWrite(LIGHT_RELAY_PIN, on ? LOW : HIGH);
  digitalWrite(LED_PIN, on ? HIGH : LOW);
  lightState = on;
}

void setFan(bool on) {
  digitalWrite(FAN_RELAY_PIN, on ? LOW : HIGH);
  fanState = on;
}

// =====================================================
// ROOT API
// =====================================================
void handleRoot() {
  server.sendHeader("Cache-Control", "no-store");
  server.send_P(200, "text/html; charset=UTF-8", MAIN_page);
}

// =====================================================
// LIGHT API
// =====================================================
void handleLightOn() {
  setLight(true);
  lightControlledByPIR = false;

  Serial.println("LIGHT ON");
  server.send(200, "text/plain", "Light ON");
}

void handleLightOff() {
  setLight(false);
  lightControlledByPIR = false;

  Serial.println("LIGHT OFF");
  server.send(200, "text/plain", "Light OFF");
}

// =====================================================
// AUTO LIGHT API
// =====================================================
void handleAutoLightOn() {
  autoLight = true;

  Serial.println("AUTO LIGHT ON");
  server.send(200, "text/plain", "Auto Light ON");
}

void handleAutoLightOff() {
  autoLight = false;
  lightControlledByPIR = false;

  Serial.println("AUTO LIGHT OFF - Manual Mode");
  server.send(200, "text/plain", "Auto Light OFF");
}

// =====================================================
// FAN API
// =====================================================
void handleFanOn() {
  setFan(true);

  if (autoFan == true) {
    fanOverride = true;
    fanOverrideStartTime = millis();
    Serial.println("FAN ON by user - Auto Fan override started");
  } else {
    Serial.println("FAN ON");
  }

  server.send(200, "text/plain", "Fan ON");
}

void handleFanOff() {
  setFan(false);

  if (autoFan == true) {
    fanOverride = true;
    fanOverrideStartTime = millis();
    Serial.println("FAN OFF by user - Auto Fan override started");
  } else {
    Serial.println("FAN OFF");
  }

  server.send(200, "text/plain", "Fan OFF");
}

// =====================================================
// AUTO FAN API
// =====================================================
void handleAutoFanOn() {
  autoFan = true;
  fanOverride = false;

  Serial.println("AUTO FAN ON");
  server.send(200, "text/plain", "Auto Fan ON");
}

void handleAutoFanOff() {
  autoFan = false;
  fanOverride = false;

  Serial.println("AUTO FAN OFF");
  server.send(200, "text/plain", "Auto Fan OFF");
}

// =====================================================
// STATUS API
// =====================================================
void handleStatus() {
  String json = "{";

  json += "\"light\":";
  json += (lightState ? "true" : "false");

  json += ",";

  json += "\"fan\":";
  json += (fanState ? "true" : "false");

  json += ",";

  json += "\"autoLight\":";
  json += (autoLight ? "true" : "false");

  json += ",";

  json += "\"autoFan\":";
  json += (autoFan ? "true" : "false");

  json += ",";

  json += "\"temp\":";
  json += String(temp, 1);

  json += ",";

  json += "\"humidity\":";
  json += String(humidity, 1);

  json += "}";

  server.send(200, "application/json", json);
}

// =====================================================
// OLED DISPLAY
// =====================================================
void updateOLED() {
  if (oledReady == false) {
    return;
  }

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("SMART HOME");

  display.setCursor(0, 16);
  display.print("Temp : ");
  display.print(temp, 1);
  display.println(" C");

  display.setCursor(0, 26);
  display.print("Hum  : ");
  display.print(humidity, 1);
  display.println(" %");

  display.setCursor(0, 42);
  display.print("Light: ");
  display.print(lightState ? "ON " : "OFF");
  display.print(" | ");
  display.println(autoLight ? "Auto" : "Manual");

  display.setCursor(0, 52);
  display.print("Fan  : ");
  display.print(fanState ? "ON " : "OFF");
  display.print(" | ");
  display.println(autoFan ? "Auto" : "Manual");

  display.display();
}

// =====================================================
// SENSOR LOGIC
// =====================================================
void readDHT() {
  if (millis() - lastDHTReadTime < dhtReadDelay) {
    return;
  }

  lastDHTReadTime = millis();

  float newHumidity = dht.readHumidity();
  float newTemp = dht.readTemperature();

  if (!isnan(newHumidity) && !isnan(newTemp)) {
    humidity = newHumidity;
    temp = newTemp;

    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.print(" C | Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    updateOLED();
  } else {
    Serial.println("Failed to read from DHT11");
  }
}

// =====================================================
// AUTO LIGHT LOGIC
// =====================================================
void handlePIRAutoLight() {
  int motion = digitalRead(PIR_PIN);

  if (motion != lastMotionState) {
    if (motion == HIGH) {
      Serial.println("Motion Detected");
    } else {
      Serial.println("No Motion");
    }

    lastMotionState = motion;
  }

  if (motion == HIGH && autoLight == true) {
    if (lightState == false) {
      setLight(true);
      lightControlledByPIR = true;

      Serial.println("PIR turned Light ON");
      updateOLED();
    }

    if (lightControlledByPIR == true) {
      lastMotionTime = millis();
    }
  } else {
    if (lightState == true && lightControlledByPIR == true) {
      if (millis() - lastMotionTime >= lightDelay) {
        setLight(false);
        lightControlledByPIR = false;

        Serial.println("Auto Light OFF after delay");
        updateOLED();
      }
    }
  }
}

// =====================================================
// AUTO FAN LOGIC
// =====================================================
void handleAutoFan() {
  if (autoFan == false) {
    return;
  }

  if (fanOverride == true) {
    if (millis() - fanOverrideStartTime >= fanOverrideDelay) {
      fanOverride = false;
      Serial.println("Fan override ended - Auto Fan resumed");
    }
  }

  if (fanOverride == true) {
    return;
  }

  if (temp >= fanOnTemp && fanState == false) {
    setFan(true);

    Serial.println("AUTO FAN ON - Temperature is high");
    updateOLED();
  }

  if (temp <= fanOffTemp && fanState == true) {
    setFan(false);

    Serial.println("AUTO FAN OFF - Temperature is low");
    updateOLED();
  }
}

// =====================================================
// SETUP HELPERS
// =====================================================
void setupPins() {
  pinMode(LED_PIN, OUTPUT);

  pinMode(PIR_PIN, INPUT);

  pinMode(LIGHT_RELAY_PIN, OUTPUT);
  digitalWrite(LIGHT_RELAY_PIN, HIGH);

  pinMode(FAN_RELAY_PIN, OUTPUT);
  digitalWrite(FAN_RELAY_PIN, HIGH);
}

void setupOLED() {
  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    oledReady = false;
    Serial.println("OLED not found");
    return;
  }

  oledReady = true;

  Serial.println("OLED started");

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Smart Home");
  display.println("Starting...");
  display.display();
}

void setupWiFi() {
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Static IP configuration failed");
  }

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  Serial.print("ESP32 MAC: ");
  Serial.println(WiFi.macAddress());
}

void setupRoutes() {
  server.on("/", handleRoot);

  server.on("/light_on", handleLightOn);
  server.on("/light_off", handleLightOff);

  server.on("/fan_on", handleFanOn);
  server.on("/fan_off", handleFanOff);

  server.on("/auto_light_on", handleAutoLightOn);
  server.on("/auto_light_off", handleAutoLightOff);

  server.on("/auto_fan_on", handleAutoFanOn);
  server.on("/auto_fan_off", handleAutoFanOff);

  server.on("/status", handleStatus);
}

// =====================================================
// SETUP
// =====================================================
void setup() {
  Serial.begin(115200);

  setupPins();

  dht.begin();
  setupOLED();

  setupWiFi();
  setupRoutes();

  server.begin();

  Serial.println("HTTP Server Started");
}

// =====================================================
// LOOP
// =====================================================
void loop() {
  server.handleClient();

  readDHT();
  handlePIRAutoLight();
  handleAutoFan();
}
