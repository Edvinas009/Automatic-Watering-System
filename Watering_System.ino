#define BLYNK_PRINT Serial
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#define BLYNK_PRINT Serial
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS D2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

char auth[] ="";               //Authentication code sent by Blynk
char ssid[] = "";                       //WiFi SSID
char pass[] = "";                       //WiFi Password

#define sensorPin D3
#define sensorPin D0
int sensorState = 0;
int lastState = 0;
int soilmoisturepercent=0;
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Blynk.virtualWrite(V5, h);  //V5 is for Humidity
  Blynk.virtualWrite(V6, t);  //V6 is for Temperature //display the moisture percent.
}
void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
   pinMode(sensorPin, INPUT);
   pinMode (D0,OUTPUT);
  dht.begin();

  timer.setInterval(1000L, sendSensor);
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
   sensors.begin();
}
int sensor=0;
void sendTemps()
{
sensor=analogRead(A0);
sensors.requestTemperatures();
float temp = sensors.getTempCByIndex(0);
Serial.println(temp);
Serial.println(sensor);
Blynk.virtualWrite(V1, temp);
Blynk.virtualWrite(V2,sensor);
delay(1000);
}
void loop()
{
  Blynk.run();
  timer.run();
  sendTemps();
  sensorState = digitalRead(sensorPin);
Serial.println(sensorState);


if (sensor < 800 ) {
  digitalWrite (D0, HIGH);
  Blynk.notify("Palaistykite augalą");

} else {
  digitalWrite (D0, LOW);
}
  delay(100);
}
