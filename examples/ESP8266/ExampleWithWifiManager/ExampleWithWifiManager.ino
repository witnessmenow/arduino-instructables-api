
 /*********************************************************************
  *  Read Instructables Author statistics from the Instructables API  *
  *  This sketch uses the WiFiManager Library for configuraiton      *
  *                                                                   *
  *  By Brian Lough                                                   *
  *  https://www.instructables.com/member/witnessmenow/instructables/ *
  *********************************************************************/

#include <InstructablesApi.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

// For storing configurations
#include "FS.h"
#include <ArduinoJson.h>

// WiFiManager Libraries
#include <DNSServer.h>            //Local DNS Server used for redirecting all rs to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

const int resetConfigPin = D8; //When high will reset the wifi manager config

char screenName[45] = "witnessmenow";

WiFiClient client;
InstructablesApi *api;

unsigned long api_delay = 5 * 60000; //time between api requests (5mins)
unsigned long api_due_time;

long subs = 0;

// flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void setup() {

  Serial.begin(115200);

  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount FS");
    return;
  }

  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
  loadConfig();

  WiFiManager wifiManager;
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  // Adding an additional config on the WIFI manager webpage for the API Key and Channel ID
  WiFiManagerParameter customScreenName("screenName", "Enter Instructables Users Name", screenName, 40);
  wifiManager.addParameter(&customScreenName);

  // If it fails to connect it will create a Instructables-Counter access point
  wifiManager.autoConnect("Instructables-Counter", "supersecret");

  strcpy(screenName, customScreenName.getValue());

  if (shouldSaveConfig) {
    saveConfig();
  }

  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  Serial.println("Init Instructables API");
  api = new InstructablesApi(client);

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

}

bool loadConfig() {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  configFile.readBytes(buf.get(), size);

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    Serial.println("Failed to parse config file");
    return false;
  }

  strcpy(screenName, json["screenName"]);
  return true;
}

bool saveConfig() {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["screenName"] = screenName;

  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  json.printTo(configFile);
  return true;
}

void forceConfigMode() {
  Serial.println("Reset");
  WiFi.disconnect();
  Serial.println("Dq");
  delay(500);
  ESP.restart();
  delay(5000);
}

void loop() {

  if (digitalRead(resetConfigPin) == HIGH) {
    forceConfigMode();
  }

  if (millis() > api_due_time)  {
    instructablesAuthorStats stats;
    stats = api->getAuthorStats(screenName);
    if(stats.error.equals(""))
    {
      Serial.println("---------Author Stats---------");
      Serial.print("Views: ");
      Serial.println(stats.views);
      Serial.print("Followers: ");
      Serial.println(stats.followersCount);
      Serial.print("Instructables Count: ");
      Serial.println(stats.instructablesCount);
      Serial.print("Featured Count: ");
      Serial.println(stats.featuredCount);

      // The following variables are available from the response (e.g. stats.collaborationsCount)
      /*
      long commentCount;
      long views;
      long featuredCount;
      long favoritesCount;
      long instructablesCount;
      long publishedCollectionsCount;
      long lessonCount;
      long courseCount;
      long topicsCount;
      long questionsCount;
      long answersCount;
      long bestAnswersCount;
      long followersCount;
      long subscriptionsCount;
      long collaborationsCount;
      String error // This will be blank if it gets in here
      */

      Serial.println("------------------------");
    } else {
      Serial.print("Error getting Author Stats: ");
      Serial.println(stats.error);
    }
    api_due_time = millis() + api_delay;
  }
}
