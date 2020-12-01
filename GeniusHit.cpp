#pragma once

#include <iostream>
#include <cpprest/json.h>
#include "Util.cpp"

using namespace std;
using namespace web;

class GeniusHit {
	string title, type, api_path;
public:
	GeniusHit(json::value json) {
		type = Util::getJSONMember("type", json);
		json::value result = Util::getJSONMemberAsJSON("result", json);
		title = Util::getJSONMember("full_title", result);
		api_path = Util::getJSONMember("api_path", result);
	}
	GeniusHit(GeniusHit& gh) {
		title = gh.title;
		type = gh.type;
		api_path = gh.api_path;
	}
	string getType() {
		return type;
	}
	string getAPIPath() {
		return api_path;
	}
	string getTitle() {
		return title;
	}
	json::value to_json() {
		json::value out;
		out[U("title")] = json::value(to_string_t(getTitle()));
		out[U("path")] = json::value(to_string_t(getAPIPath()));
		return out;
	}
};