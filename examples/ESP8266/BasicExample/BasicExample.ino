/*********************************************************************
 *  Read Instructables Author statistics from the Instructables API  *
 *                                                                   *
 *  By Brian Lough                                                   *
 *  https://www.instructables.com/member/witnessmenow/instructables/ *
 *********************************************************************/

#include <InstructablesApi.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include <ArduinoJson.h> // This Sketch doesn't technically need this, but the library does so it must be installed.

//------- Replace the following! ------
char ssid[] = "xxx";       // your network SSID (name)
char password[] = "yyyy";  // your network key
#define SCREEN_NAME "witnessmenow" //Replace with your instructables user name
#define INSTRUCABLE_ID "EBB44Y4IZT6HFIH"
// view source of an instructable page and search for
// "LogHit('" , the ID will be the first string after that

WiFiClient client;
InstructablesApi api(client);

unsigned long api_delay = 5 * 60000; //time between api requests (5mins)
unsigned long api_due_time;

void setup() {

  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);


}

void loop() {

  if (millis() > api_due_time)  {
    instructablesAuthorStats stats;
    stats = api.getAuthorStats(SCREEN_NAME);
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

    instructableStats postStats;
    postStats = api.getInstructableStats(INSTRUCABLE_ID);
    if(postStats.error.equals(""))
    {
      Serial.println("---------Post Stats---------");
      Serial.print("Views: ");
      Serial.println(postStats.views);
      Serial.print("Favorites: ");
      Serial.println(postStats.favorites);
      Serial.print("Comments: ");
      Serial.println(postStats.comments);
      Serial.println("------------------------");
    } else {
      Serial.print("Error getting Post Stats: ");
      Serial.println(postStats.error);
    }

    api_due_time = millis() + api_delay;
  }
}
