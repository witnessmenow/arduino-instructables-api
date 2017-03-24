/*********************************************************************
 *  Read Instructables Author statistics from the Instructables API  *
 *                                                                   *
 *  By Brian Lough                                                   *
 *  https://www.instructables.com/member/witnessmenow/instructables/ *
 *********************************************************************/

#include <InstructablesApi.h>
#include <Ethernet.h>
#include <SPI.h>

#include <ArduinoJson.h> // This Sketch doesn't technically need this, but the library does so it must be installed.

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//------- Replace the following! ------
char ssid[] = "Mikrotik";       // your network SSID (name)
char password[] = "carolinebrian";  // your network key
#define SCREEN_NAME "witnessmenow" //Replace with your instructables user name
#define INSTRUCABLE_ID "EBB44Y4IZT6HFIH"
// view source of an instructable page and search for
// "LogHit('" , the ID will be the first string after that

EthernetClient client;
InstructablesApi api(client);

unsigned long api_delay = 5 * 60000; //time between api requests (5mins)
unsigned long api_due_time;

void setup() {

  Serial.begin(9600);

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  }

  delay(1000);


}

void showAuthorStats(String screenName) {
  instructablesAuthorStats stats;
  stats = api.getAuthorStats(screenName);
  if(stats.error.equals("")) {
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
}

void showPostStats(String postId) {
  instructableStats postStats;
  postStats = api.getInstructableStats(postId);
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
}

void loop() {

  if (millis() > api_due_time)  {

    showPostStats(INSTRUCABLE_ID);

    api_due_time = millis() + api_delay;
  }
}
