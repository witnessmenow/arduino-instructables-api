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

String InstructablesApi::sendGetToInstructables(String command) {
	String headers="";
	String body="";
	bool finishedHeaders = false;
	bool currentLineIsBlank = true;
	unsigned long now;
	bool avail;
	// Connect with Instructables api with http
	if (client->connect(INSTRUCTABLES_HOST, INSTRUCTABLES_PORT)) {

		if(_debug) Serial.println(".... connected to server");

		char c;
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


				if(_debug) Serial.write(c);

				if(!finishedHeaders){
					if (currentLineIsBlank && c == '\n') {
						finishedHeaders = true;
					}
					else{
						headers = headers + c;

					}
				} else {
					if (ch_count < maxMessageLength)  {
						body=body+c;
						ch_count++;
					}
				}

				if (c == '\n') {
					currentLineIsBlank = true;
				}else if (c != '\r') {
					currentLineIsBlank = false;
				}
			}
		}
	}

	closeClient();
	return body;
}

instructableStats InstructablesApi::getInstructableStats(String instructableId){
	String command="/json-api/getIbleStats?id="+instructableId;

	if(_debug) Serial.println(command);

	String response = sendGetToInstructables(command);       //receieve reply from Instructables
	DynamicJsonDocument  jsonBuffer(1024);
	DeserializationError error = deserializeJson(jsonBuffer, response);
	instructableStats stats;
		if (error) {
			stats.error = error.code();
		} else {
			stats.comments = jsonBuffer["comments"].as<long>();
			stats.views = jsonBuffer["views"].as<long>();
			stats.favorites =jsonBuffer["favorites"].as<long>();
			stats.error = "";
		}
	return stats;
}

instructablesAuthorStats InstructablesApi::getAuthorStats(String screenName){
	String command="/json-api/showAuthorStats?screenName="+screenName;

	if(_debug) Serial.println(command);

	String response = sendGetToInstructables(command);       //receieve reply from Instructables
	DynamicJsonDocument  jsonBuffer(1024);
	DeserializationError error = deserializeJson(jsonBuffer, response);
	instructablesAuthorStats stats;
		if (error) {
			stats.error = error.code();
		} else {
			stats.commentCount = jsonBuffer["commentCount"].as<long>();
			stats.views = jsonBuffer["views"].as<long>();
			stats.featuredCount = jsonBuffer["featuredCount"].as<long>();
			stats.favoritesCount = jsonBuffer["favoritesCount"].as<long>();
			stats.instructablesCount = jsonBuffer["instructablesCount"].as<long>();
			stats.publishedCollectionsCount = jsonBuffer["publishedCollectionsCount"].as<long>();
			stats.lessonCount = jsonBuffer["lessonCount"].as<long>();
			stats.courseCount = jsonBuffer["courseCount"].as<long>();
			stats.topicsCount = jsonBuffer["topicsCount"].as<long>();
			stats.questionsCount = jsonBuffer["questionsCount"].as<long>();
			stats.answersCount = jsonBuffer["answersCount"].as<long>();
			stats.bestAnswersCount = jsonBuffer["bestAnswersCount"].as<long>();
			stats.followersCount = jsonBuffer["followersCount"].as<long>();
			stats.subscriptionsCount = jsonBuffer["subscriptionsCount"].as<long>();
			stats.collaborationsCount = jsonBuffer["collaborationsCount"].as<long>();
			stats.error = "";
		}

	return stats;
}

void InstructablesApi::closeClient() {
  if(client->connected()){
    client->stop();
  }
}
