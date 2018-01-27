# arduino-instructables-api

Get Instructables stats directly on your Arduino!

![Instructables Library Example](https://i.imgur.com/PDVRcCy.png)

Check out the examples for how to use it, no API key needed.

A wrapper for the undocumented Instructables.com API that their website uses to populate data

## Installation

Install from the Arduino Library Manager or download the Zip from here and add that way

Requires [Arduino Json by bblanchon](https://github.com/bblanchon/ArduinoJson), which can also be installed from the library manager.

## Currently Supports:

#### Author Stats
```
struct instructablesAuthorStats{
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
  String error;
};
```

#### Instructable Post Stats
```
struct instructableStats{
  long comments;
  long views;
  long favorites;
  String error;
};
```


## Supported Boards:

#### Tested Boards:
- ESP8266
- ESP32

### Should/Could Work:
- WiFi101 Boards (MKR1000 etc)

#### Might Work
- Ethernet Shields (Might not have enough memory for Author Stats)
