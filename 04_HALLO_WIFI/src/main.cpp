#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <SSD1306.h>
#include <WiFiManager.h>

// Sensor
#define DHTPIN 5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Display
#define SDA 14
#define SCL 12

SSD1306 display(0x3c, SDA, SCL);

// WifiManager
WiFiManager wifi;

void setup()
{
  Serial.begin(115200);
  dht.begin();
  display.init();
  display.setFont(ArialMT_Plain_10);

  bool response;
  response = wifi.autoConnect("ArduinoFlo", "04032000");
  if (response)
  {
    Serial.println("Verbindung hergestellt");
  }
  else
  {
    Serial.println("Verbindung fehlgeschlagen");
  }
}

void loop()
{
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t))
  {
    Serial.println(F("Fehler beim Abfragen der Werte vom DHT Sensor!"));
    return;
  }

  // Serial.print(F("Luftfeuchtigkeit: "));
  // Serial.print(h);
  // Serial.print(F("% Temperatur: "));
  // Serial.print(t);
  // Serial.println(F("°C "));

  display.clear();
  display.drawString(0, 0, "Wohnzimmer");
  display.drawString(0, 12, "Feuchtigkeit : " + String(h, 2) + " %");
  display.drawString(0, 22, "Temperatur : " + String(t, 2) + "°C");
  display.drawString(0, 42, "IP: " + WiFi.localIP().toString());
  display.display();
}