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


#include "InstructablesApi.h"

InstructablesApi::InstructablesApi(Client &client)	{
	this->client = &client;
}

void InstructablesApi::sendGetToInstructables(String command, char *response) {
	bool finishedHeaders = false;
	bool currentLineIsBlank = true;
	unsigned long now;
	bool avail;
	// Connect with Instructables api with http
	if (client->connect(INSTRUCTABLES_HOST, INSTRUCTABLES_PORT)) {
		Serial.println(".... connected to server");
		String a="";
		int ch_count=0;
		client->println("GET " + command + " HTTP/1.1");
		client->println("Host:  " INSTRUCTABLES_HOST);
		client->println("User-Agent:  arduino/1.0.0");
		client->println("Accept:  */*");
		client->println("");
		now=millis();
		avail=false;
		while (millis() - now < INSTRUCTABLES_TIMEOUT) {
			while (client->available()) {

				// Allow body to be parsed before finishing
				avail = finishedHeaders;
				char c = client->read();
				//Serial.write(c);

				if(!finishedHeaders){
					if (currentLineIsBlank && c == '\n') {
						finishedHeaders = true;
					}
					else{
						// headers = headers + c;

					}
				} else {
					if (ch_count < maxMessageLength)  {
						response[ch_count] = c;
						ch_count++;
					}
				}

				if (c == '\n') {
					currentLineIsBlank = true;
				}else if (c != '\r') {
					currentLineIsBlank = false;
				}
			}
			if (avail) {
				//Serial.println("Body:");
				//Serial.println(body);
				//Serial.println("END");
				break;
			}
		}
	}
}

instructableStats InstructablesApi::getInstructableStats(String instructableId){
	String command="/json-api/getIbleStats?id="+instructableId;
	Serial.println(command);
  char response[INSTRUCTABLES_STATS_RESPONSE_LIMIT];
	sendGetToInstructables(command, response);       //recieve reply from Instructables
	Serial.println(response);
  DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(response);
	instructableStats stats;
	if (root.success()) {
		if (root.containsKey("error")) {
			stats.error = root["error"].as<String>();
		} else {
			stats.comments = root["comments"].as<long>();
			stats.views = root["views"].as<long>();
			stats.favorites = root["favorites"].as<long>();
			stats.error = "";
		}
	} else {
		stats.error = "Library: Failed to parse response";
	}

	return stats;
}

instructablesAuthorStats InstructablesApi::getAuthorStats(String screenName){
	String command="/json-api/showAuthorStats?screenName="+screenName;
	Serial.println(command);
  char response[INSTRUCTABLES_STATS_RESPONSE_LIMIT];
	sendGetToInstructables(command, response);       //recieve reply from Instructables
	Serial.println(response);
  DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(response);
	instructablesAuthorStats stats;
	if (root.success()) {
		if (root.containsKey("error")) {
			stats.error = root["error"].as<String>();
		} else {
			stats.commentCount = root["commentCount"].as<long>();
			stats.views = root["views"].as<long>();
			stats.featuredCount = root["featuredCount"].as<long>();
			stats.favoritesCount = root["favoritesCount"].as<long>();
			stats.instructablesCount = root["instructablesCount"].as<long>();
			stats.publishedCollectionsCount = root["publishedCollectionsCount"].as<long>();
			stats.lessonCount = root["lessonCount"].as<long>();
			stats.courseCount = root["courseCount"].as<long>();
			stats.topicsCount = root["topicsCount"].as<long>();
			stats.questionsCount = root["questionsCount"].as<long>();
			stats.answersCount = root["answersCount"].as<long>();
			stats.bestAnswersCount = root["bestAnswersCount"].as<long>();
			stats.followersCount = root["followersCount"].as<long>();
			stats.subscriptionsCount = root["subscriptionsCount"].as<long>();
			stats.collaborationsCount = root["collaborationsCount"].as<long>();
			stats.error = "";
		}
	} else {
		stats.error = "Library: Failed to parse response";
	}

	return stats;
}
