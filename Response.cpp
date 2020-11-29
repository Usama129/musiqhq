#pragma once
#include <iostream>
#include <cpprest/json.h>
#include "Util.cpp"

using namespace std;
using namespace web;

class Hit {
protected:
	string title;
public:
	Hit() {}
	virtual json::value to_json() = 0;
	Hit(string title) {
		Hit::title = title;
	}
	string getTitle() {
		return title;
	}
};

class Response {
	string status;
	vector<Hit*> hits;
public:
	Response(string status, vector<Hit*> hits) {
		Response::status = status;
		Response::hits = hits;
	};
	json::value to_json() {
		json::value out;
		for (int i = 0; i < hits.size(); i++) {
			out[i] = hits[i]->to_json();
		}
		return out;
	}
};

class GeniusHit : public Hit {
	string type, api_path;
public:
	GeniusHit(json::value json) {
		type = Util::getJSONMember("type", json);
		json::value result = Util::getJSONMemberAsJSON("result", json);
		title = Util::getJSONMember("full_title", result);
		api_path = Util::getJSONMember("api_path", result);
	}
	string getType() {
		return type;
	}
	string getAPIPath() {
		return api_path;
	}
	json::value to_json() {
		json::value out;
		out[U("title")] = json::value(to_string_t(getTitle()));
		return out;
	}
};