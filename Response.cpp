#pragma once
#include <iostream>
#include <utility>
#include <cpprest/json.h>
#include <cstring>
#include "Util.cpp"
#include "GeniusHit.cpp"
#include <cpprest\http_client.h>

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace utility::conversions;

class Response {
protected:
	string status, error;
public:
	Response() {}
	Response(string status) {
		Response::status = status;
	};
	//Response(string status, string html) : hits({}), status(status), html(html) {}
	virtual json::value to_json() = 0;
	
};

class SearchResponse : public Response {
	vector<GeniusHit*> hits;
public:
	SearchResponse() {}
	SearchResponse(string status, vector<GeniusHit*> hits) : hits(hits), Response(status) {};
	// copy constructor has to be added because this class contains 'hits' pointer vector, which cant be copied otherwise
	SearchResponse(SearchResponse& sr) {
		status = sr.status;
		for (auto hit : sr.hits) {
			SearchResponse::hits.push_back(new GeniusHit(*hit));
		}
	}
	SearchResponse(json::value json) {
		parseFromJSON(json);
	}
	~SearchResponse() {
		for (auto hit : hits) {
			delete hit;
		}
		hits.clear();
	}
	json::value to_json() {
		json::value out;
		if (error.empty()) {
			for (int i = 0; i < hits.size(); i++) {
				out[i] = hits[i]->to_json();
			}
		}
		else {
			out[to_string_t("error")] = json::value(to_string_t("API Error"));
		}
		return out;
	}
	bool parseFromJSON(json::value json) {
		json::value meta = Util::getJSONMemberAsJSON("meta", json);
		status = Util::getJSONMember("status", meta);
		if (status != "200") {
			error = "200 not received";
			status = string(status_codes::InternalError + "");
			return false;
		}
		json::value res = Util::getJSONMemberAsJSON("response", json);
		json::value hitsJson = Util::getJSONMemberAsJSON("hits", res);

		json::array hitsArray = hitsJson.as_array();

		for (int i = 0; i < hitsArray.size(); i++) {
			GeniusHit* obj = new GeniusHit(hitsArray.at(i));
			hits.push_back(obj);
		}

		return true;
	}
};

class LyricsResponse : public Response {
	string title, lyrics;
public:
	LyricsResponse(json::value json) {
		parseFromJSON(json);
	}
	bool parseFromJSON(json::value json) {
		status = Util::getJSONMember("status", json);
		if (status != "success") {
			json::value err = Util::getJSONMemberAsJSON("error", json);
			error = Util::getJSONMember("error_message", err);
			status = string(status_codes::InternalError + "");
			return false;
		}
		json::array result = Util::getJSONMemberAsJSON("result", json).as_array();
		title = Util::getJSONMember("full_title", result.at(0));
		lyrics = Util::getJSONMember("lyrics", result.at(0));
		return true;
	}
	json::value to_json() {
		json::value out;
		if (error.empty()) {
			out[to_string_t("title")] = json::value(to_string_t(title));
			out[to_string_t("lyrics")] = json::value(to_string_t(lyrics));
		}
		else {
			out[to_string_t("error")] = json::value(to_string_t("API Error"));
		}
		return out;
	}
};

class DescriptionResponse : public Response {
	string title, description;
public:
	DescriptionResponse(json::value json) {
		parseFromJSON(json);
	}
	bool parseFromJSON(json::value json) {
		json::value meta = Util::getJSONMemberAsJSON("meta", json);
		status = Util::getJSONMember("status", meta);
		if (status != "200") {
			error = "200 not received";
			status = string(status_codes::InternalError + "");
			return false;
		}
		json::value res = Util::getJSONMemberAsJSON("response", json);
		json::value song = Util::getJSONMemberAsJSON("song", res);

		title = Util::getJSONMember("full_title", song);
		json::value desc = Util::getJSONMemberAsJSON("description", song);
		description = Util::getJSONMember("plain", desc);
		return true;
	}
	json::value to_json() {
		json::value out;
		if (error.empty()) {
			out[to_string_t("title")] = json::value(to_string_t(title));
			out[to_string_t("description")] = json::value(to_string_t(description));
		}
		else {
			out[to_string_t("error")] = json::value(to_string_t("API Error"));
		}
		return out;
	}
};