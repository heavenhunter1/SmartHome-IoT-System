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
  server.send(200, "text/plain", "ESP32 is online");
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
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
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
