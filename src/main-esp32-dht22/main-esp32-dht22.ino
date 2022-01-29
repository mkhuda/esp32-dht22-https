/*
  Mkhuda (2022)
  Test project on ESP32 and DHT22 for sending data 
  through Secure HTTPS Post to the custom server host
*/

// Import required libraries
#include <Arduino.h>
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "DHT.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>

// define led blink
#define LED 2

// dht11 / dht22 pin
#define DHTPIN 15

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Set your access point network credentials
const char *ssid = "esp32_0";
const char *password = "keepomap";

const char *localhost = "192.168.1.2";

// for wifi internet access
char fs_ssid[100] = {'\0'};
char fs_password[100] = {'\0'};

const String PATH_WIFI_SSID = "/wifissid.txt";
const String PATH_WIFI_PASSWORD = "/wifipassword.txt";

const String apiKey = ""; // TODO: should be for x-api-key header value
String deviceSerial = ""; // TODO: should be deviceSerial
String host = "https://test.test.com";
String postUrl = host + "/v1/keepomap/sensor/" + deviceSerial; // Post path for URL

// This is letsencrypt root CA
const char *rootCa =
    "-----BEGIN CERTIFICATE-----\n"
    "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\n"
    "-----END CERTIFICATE-----\n";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

long ledTimeMillis = 0;
unsigned long ledTimeSentDelay = 5000; // interval of Sent data to server

// DHT Setup
long previousDHT22Millis = 0;
unsigned long timerDHT22SentDelay = 30000; // 30s interval of Sent data to server
DHT dht(DHTPIN, DHTTYPE);

// used to handle Wifi SSID / Password on ESP32 Acces Point server
// You can change Wifi setup through connection to this ESP32, and go to 192.198.4.1 for Web server UI
void handleSubmitWifi(AsyncWebServerRequest *request)
{
  StaticJsonDocument<50> doc;
  String payloadOutput;
  if (request->method() != HTTP_GET)
  {
    doc["status"] = 0;
    serializeJson(doc, payloadOutput);
    request->send(200, "application/json", payloadOutput);
  }
  else
  {
    String message = "POST form was:\n";
    for (uint8_t i = 0; i < request->args(); i++)
    {
      message += " " + request->argName(i) + ": " + request->arg(i) + "\n";
      if (request->argName(i) == "ssid")
      {
        fsWrite(PATH_WIFI_SSID, request->arg(i));
      }
      else if (request->argName(i) == "password")
      {
        fsWrite(PATH_WIFI_PASSWORD, request->arg(i));
      }
    }

    doc["status"] = 1;
    serializeJson(doc, payloadOutput);
    request->send(200, "application/json", payloadOutput);
  }
}

// used to handle reboot on ESP32 Acces Point server
void handleReboot(AsyncWebServerRequest *request)
{
  StaticJsonDocument<30> doc;
  String payloadOutput;
  doc["status"] = 1;
  serializeJson(doc, payloadOutput);
  request->send(200, "application/json; charset=utf-8", payloadOutput);
  delay(1000);
  ESP.restart();
}

// used to gather data on ESP32 Access Point server
void handleSysInfo(AsyncWebServerRequest *request)
{
  String result;
  int wifiStatus;
  if (WiFi.status() == WL_CONNECTED)
  {
    wifiStatus = 1;
  }
  else
  {
    wifiStatus = 0;
  }

  StaticJsonDocument<300> doc;
  String payloadOutput;
  doc["deviceSerial"] = String(deviceSerial);
  doc["ip"] = WiFi.localIP();
  doc["connection"] = wifiStatus;
  doc["ssid"] = String(fs_ssid);
  doc["password"] = String(fs_password);
  serializeJson(doc, payloadOutput);

  request->send(200, "application/json; charset=utf-8", payloadOutput);
}

// Set right clock setting to make sure SSL are ready for the server
void setClock()
{
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print(F("Waiting for NTP time sync: "));
  time_t nowSecs = time(nullptr);
  while (nowSecs < 8 * 3600 * 2)
  {
    delay(500);
    Serial.print(F("."));
    yield();
    nowSecs = time(nullptr);
  }

  Serial.println();
  struct tm timeinfo;
  gmtime_r(&nowSecs, &timeinfo);
  Serial.print(F("Current time: "));
  Serial.print(asctime(&timeinfo));
}

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);
  SPIFFS.begin();
  Serial.println();
  delay(2000);
  getWifiSSID_FS();
  getWifiPassword_FS();
  dht.begin();
  pinMode(LED, OUTPUT);

  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.println("AP IP address: ");
  Serial.println(IP);
  // Start server
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  server.begin();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/web/index.html"); });
  server.on("/favicon.png", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/web/favicon.png"); });
  server.on("/global.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/web/global.css"); });
  server.on("/build/bundle.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/web/build/bundle.css"); });
  server.on("/build/bundle.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/web/build/bundle.js"); });
  server.on("/submit-wifi", HTTP_GET, handleSubmitWifi);
  server.on("/reboot", HTTP_GET, handleReboot);
  server.on("/sysinfo", HTTP_GET, handleSysInfo);
  WiFi.begin(fs_ssid, fs_password);

  setClock();
}

void loop()
{
  led();

  if ((WiFi.status() == WL_CONNECTED))
  { // Check the current connection status
    readDHT22Sensor();
  }
}

// Just blinking the Builtin LED,
// - if it fast (100ms) then connection is Ok
// - if slow (4500ms / 4,5 second) then there are issue on Wifi connection
void led()
{
  if (millis() - ledTimeMillis > ledTimeSentDelay)
  {
    // DHT11 begin loop, not using force mode (false)
    if ((WiFi.status() == WL_CONNECTED))
    {
      digitalWrite(LED, HIGH);
      delay(100);
      digitalWrite(LED, LOW);
      delay(4500);
    }
    else
    {
      digitalWrite(LED, HIGH);
      delay(4500);
      digitalWrite(LED, LOW);
      delay(500);
    }
    ledTimeMillis = millis();
  }
}

// Get wifi SSID from data on setup
void getWifiSSID_FS()
{
  fsRead(PATH_WIFI_SSID, fs_ssid);
}

// Get wifi password from data on setup
void getWifiPassword_FS()
{
  fsRead(PATH_WIFI_PASSWORD, fs_password);
}

// Used to read data on SPPIFS
void fsRead(String filePath, char *fs_char)
{
  File file = SPIFFS.open(filePath, "r");
  if (file)
  {
    uint16_t i = 0;
    while (file.available())
    {
      fs_char[i] = file.read();
      i++;
    }
    fs_char[i] = '\0';
    file.close();
  }
  else
  {
    Serial.println("error fs read on: " + filePath);
  }
}

// Used to save file data on flash
void fsWrite(String filePath, String value)
{
  // Open the file
  File file = SPIFFS.open(filePath, "w");
  // Write to the file
  file.print(value);
  delay(1);
  // Close the file
  file.close();
}

// Post server using WiFiClientSecure with Letsencrypt root ca.
void postToServer(String payloadPost)
{
  WiFiClientSecure *client = new WiFiClientSecure;
  if (client)
  {
    client->setCACert(rootCa);
    // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is
    HTTPClient https;

    Serial.print("[HTTPS] begin...\n");
    const char *url = postUrl.c_str();
    Serial.println(url);
    if (https.begin(*client, url))
    { // HTTPS
      https.addHeader("Content-Type", "application/json");
      https.addHeader("X-Api-Key", String(apiKey));

      // Send HTTP POST request with payloadPost from DHT22
      int httpResponseCode = https.POST(payloadPost);

      // httpCode will be negative on error
      if (httpResponseCode > 0)
      {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] POST... code: %d\n", httpResponseCode);

        // file found at server
        if (httpResponseCode == HTTP_CODE_OK || httpResponseCode == HTTP_CODE_MOVED_PERMANENTLY)
        {
          String payload = https.getString();
          Serial.println(payload);
        }
      }
      else
      {
        Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpResponseCode).c_str());
      }

      https.end();
    }
    else
    {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }

  delete client;
}

// The function to read DHT22 sensor and send payload to server
void readDHT22Sensor()
{
  if (millis() - previousDHT22Millis > timerDHT22SentDelay)
  {
    previousDHT22Millis = millis();
    // DHT11 begin loop, not using force mode (false)
    if (dht.read(false))
    {
      StaticJsonDocument<50> doc;
      String payloadOutput;
      float temperature = dht.readTemperature(false, false);
      float humidity = dht.readHumidity(false);
      float heat = dht.computeHeatIndex(temperature, humidity, false);
      doc["temperature"] = floor(temperature * 100 + 0.5) / 100;
      doc["humidity"] = floor(humidity * 100 + 0.5) / 100;
      doc["heat"] = floor(heat * 100 + 0.5) / 100;
      serializeJson(doc, payloadOutput);
      Serial.println(payloadOutput);
      postToServer(payloadOutput);
      //      const char* path = postPath.c_str();
      //      postURL(host, 443, path, payloadOutput);
    }
    else
    {
      Serial.print("Error DHT22 read\n");
    }
  }
}
