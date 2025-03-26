// DHT Temperature & Humidity Sensor
// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include <WiFi.h>
#include <WebServer.h>

const char *ssid = "Wokwi-GUEST";
const char *password = "";
const bool WOKWI = true;
String temp;

sensors_event_t event;
WebServer server(80);

// For reading files uploaded using SPIFFS
#include "SPIFFS.h"

#define DHTPIN 13 // Digital pin connected to the DHT sensor

// #define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22 // DHT 22 (AM2302)  *
// #define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS; // It can only hold non-negative values

void setup()
{
  // Initialize device.
  Serial.begin(9600);

  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  File file = SPIFFS.open("/index.html");
  if (!file)
  {
    Serial.println("Failed to open file for reading");
    return;
  }
  /*
  String fileContent = "";
  Serial.println("File Content:");
  while (file.available())
  {
    fileContent += (char)file.read();
  }
  file.close();
  Serial.println(fileContent);
  */

  // Humudity and Temperature sensor setup
  dht.begin();
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print(F("Sensor Type: "));
  Serial.println(sensor.name);
  Serial.print(F("Driver Ver:  "));
  Serial.println(sensor.version);
  Serial.print(F("Unique ID:   "));
  Serial.println(sensor.sensor_id);
  Serial.print(F("Max Value:   "));
  Serial.print(sensor.max_value);
  Serial.println(F("°C"));
  Serial.print(F("Min Value:   "));
  Serial.print(sensor.min_value);
  Serial.println(F("°C"));
  Serial.print(F("Resolution:  "));
  Serial.print(sensor.resolution);
  Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print(F("Sensor Type: "));
  Serial.println(sensor.name);
  Serial.print(F("Driver Ver:  "));
  Serial.println(sensor.version);
  Serial.print(F("Unique ID:   "));
  Serial.println(sensor.sensor_id);
  Serial.print(F("Max Value:   "));
  Serial.print(sensor.max_value);
  Serial.println(F("%"));
  Serial.print(F("Min Value:   "));
  Serial.print(sensor.min_value);
  Serial.println(F("%"));
  Serial.print(F("Resolution:  "));
  Serial.print(sensor.resolution);
  Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;

  // Set up WEB-Server
  // Connect to WiFi in STA
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");

  server.begin();
  Serial.println("Server started");

  Serial.print("ESP IP Address: ");
  Serial.println(WiFi.localIP());
  // On Wokwi Simulation
  if (WOKWI)
  {
    Serial.println("WebPage: http://localhost:PORTNUMBER/\nThe PORTNUMBER is set inside wokwi.toml ");
  }

  // Setup web
  server.on("/", HTTP_GET, []() {    
    File file = SPIFFS.open("/index.html");
    server.streamFile(file, "text/html");
    file.close();
  });

  // Store json data
  server.on("/data", HTTP_GET, []() {
    String json = "{\"temp\":" + temp + ",\"humidity\":" + event.relative_humidity + "}";
    server.send(200, "application/json", json);
  });
  
  server.onNotFound([]() {
    server.send(404, "text/plain", "Not found");
  });
  
}

unsigned long previousMillis = 0;

void loop()
{
  unsigned long currentMillis = millis();

  // Handle server's client
  server.handleClient();

  // Delay between measurements.
  if (currentMillis - previousMillis >= delayMS)
  {
    previousMillis = currentMillis;

    // Get temperature event and print its value.

    dht.temperature().getEvent(&event);
    if (isnan(event.temperature))
    {
      Serial.println(F("Error reading temperature!"));
    }
    else
    {
      temp = String(event.temperature);
      Serial.print(F("Temperature: "));
      Serial.print(temp);
      Serial.println(F("°C"));
    }

    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity))
    {
      Serial.println(F("Error reading humidity!"));
    }
    else
    {
      Serial.print(F("Humidity: "));
      Serial.print(event.relative_humidity);
      Serial.println(F("%"));
    }
  }
}