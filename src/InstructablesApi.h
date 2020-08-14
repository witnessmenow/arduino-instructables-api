/*
Copyright (c) 2017 Brian Lough. All right reserved.

InstructableApi - An Arduino wrapper for the Instructables API

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/


#ifndef InstructablesApi_h
#define InstructablesApi_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Client.h>


#define INSTRUCTABLES_HOST "www.instructables.com"
#define INSTRUCTABLES_PORT 443
#define INSTRUCTABLES_TIMEOUT 1500

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

struct instructableStats{
  long comments;
  long views;
  long favorites;
  String error;
};

class InstructablesApi
{
  public:
    InstructablesApi (Client &client);
    String sendGetToInstructables(String command);
    instructablesAuthorStats getAuthorStats(String screenName);
    instructableStats getInstructableStats(String instructableId);
    bool _debug = false;

  private:
    Client *client;
    const int maxMessageLength = 1000;
    void closeClient();
};

#endif
