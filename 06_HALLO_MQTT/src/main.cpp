#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <SSD1306.h>
#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <ArduinoMqttClient.h>

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

// WebServer
ESP8266WebServer server(80);

float h;
float t;

// MqqtClient
const char broker[] = "10.0.1.142";
int port = 1883;

const char topic_t[] = "esp/temp";
const char topic_h[] = "esp/humi";

WiFiClient espClient;
MqttClient mqttClient(espClient);

String createHTML(float temperature, float humidity)
{
  String html = "<!DOCTYPE html> <html>\n";
  html += "<html>";
  html += "<head>";
  // html += "<meta http-equiv = \"refresh\" content = \"2\">";
  html += "<title>Wohnzimmer DHT</title>";
  html += "</head>";
  html += "<body>";
  html += "<h1>Wohnzimmer DHT</h1>";
  html += "<p>Luftfeuchtigkeit: ";
  html += humidity;
  html += "%</p>";
  html += "<p>Temperatur: ";
  html += temperature;
  html += "°C</p>";
  html += "</body>";
  html += "</html>";
  return html;
}

void handleRoot()
{
  server.send(200, "text/html", createHTML(t, h));
}

void handleTemperature()
{
  server.send(200, "text/plain", String(t, 2));
}

void handleHumidity()
{
  server.send(200, "text/plain", String(h, 2));
}

void handleNotFound()
{
  server.send(404, "text/plain", "Die angeforderte Ressource wurde nicht gefunden.");
}

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

  server.on("/", handleRoot);
  server.on("/t", handleTemperature);
  server.on("/h", handleHumidity);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop()
{
  server.handleClient();

  if (millis() % 2000 == 0)
  {

    h = dht.readHumidity();
    t = dht.readTemperature();

    if (isnan(h) || isnan(t))
    {
      Serial.println(F("Fehler beim Abfragen der Werte vom DHT Sensor!"));
      return;
    }

    display.clear();
    display.drawString(0, 0, "Wohnzimmer");
    display.drawString(0, 12, "Feuchtigkeit : " + String(h, 2) + " %");
    display.drawString(0, 22, "Temperatur : " + String(t, 2) + "°C");
    display.drawString(0, 42, "IP: " + WiFi.localIP().toString());
    display.display();

    if (!mqttClient.connected())
    {
      mqttClient.connect(broker, port);
    }

    mqttClient.beginMessage(topic_t);
    mqttClient.print(String(t, 2));
    mqttClient.endMessage();

    mqttClient.beginMessage(topic_h);
    mqttClient.print(String(h, 2));
    mqttClient.endMessage();
  }
}