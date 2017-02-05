#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
 
// replace with your channel’s thingspeak API key and your SSID and password
String apiKey = "W7E8CCMTPYYSG5MC";
const char* ssid = "Taylor_home";
const char* password = "58FF675BEFD682A329B4";
const char* server = "api.thingspeak.com";

int compass;

 
WiFiClient client;
 
void setup() 
{
Serial.begin(115200);
Serial.println("Booting");
WiFi.mode(WIFI_STA);
delay(10);
pinMode(14, INPUT_PULLUP);
pinMode(12, INPUT_PULLUP);
pinMode(13, INPUT_PULLUP);
pinMode(0, INPUT);
pinMode(2, INPUT);

 
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

  //setup pluviometer
 
}
 
void loop() 
{
ArduinoOTA.handle();
compass = 0;
if(digitalRead(14)== LOW)
{
bitSet(compass, 0);
}
if(digitalRead(12)== LOW) //pin D6
{
bitSet(compass, 1);
}
if(digitalRead(13)== LOW) //pin D7
{
bitSet(compass, 2);
}
if(digitalRead(0)== LOW)
{
bitSet(compass, 3);
}
if(digitalRead(2)== LOW)
{
bitSet(compass, 4);
}

Serial.println(compass);
delay(1000);
}

