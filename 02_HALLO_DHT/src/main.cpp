#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(115200);
  dht.begin();
}

void loop()
{
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t))
  {
    Serial.println(F("Feheler beim Abfragen dder Werte vom DHT Sensor!"));
    return;
  }

  Serial.print(F("Luftfeuchtigkeit: "));
  Serial.print(h);
  Serial.print(F("% Temperatur: "));
  Serial.print(t);
  Serial.println(F("°C "));
}