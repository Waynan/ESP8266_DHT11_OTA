#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
 
// replace with your channel’s thingspeak API key and your SSID and password
String apiKey = "thingspeak api key";
const char* ssid = "Androidw";
const char* password = "87654321";
const char* server = "api.thingspeak.com";
 
#define DHTPIN D2
#define DHTTYPE DHT11 
 
DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;
 
void setup() 
{
Serial.begin(115200);
Serial.println("Booting");
WiFi.mode(WIFI_STA);
delay(10);
dht.begin();
 
Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);

WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

Serial.println("");
Serial.println("WiFi connected");

 // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("Weatherstation");

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 
}
 
void loop() 
{
ArduinoOTA.handle();
float h = dht.readHumidity();
float t = dht.readTemperature();
if (isnan(h) || isnan(t)) 
{
Serial.println("Failed to read from DHT sensor!");
return;
}
 
if (client.connect(server,80)) {
String postStr = apiKey;
postStr +="&field1=";
postStr += String(t);
postStr +="&field2=";
postStr += String(h);
postStr += "\r\n\r\n";
 
client.print("POST /update HTTP/1.1\n");
client.print("Host: api.thingspeak.com\n");
client.print("Connection: close\n");
client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
client.print("Content-Type: application/x-www-form-urlencoded\n");
client.print("Content-Length: ");
client.print(postStr.length());
client.print("\n\n");
client.print(postStr);
 
Serial.print("Temperature: ");
Serial.print(t);
Serial.print(" degrees Celsius Humidity: ");
Serial.print(h);
Serial.println("Sending data to Thingspeak");
}
client.stop();
 
Serial.println("Waiting 30 secs");
// thingspeak needs at least a 15 sec delay between updates
// 20 seconds to be safe
delay(30000);
}


