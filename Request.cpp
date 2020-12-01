#pragma once

#include <iostream>
#include <cpprest/json.h>
#include "Util.cpp"
#include "Response.cpp"
#include "Genius.h"
#include "audd.h"

using namespace std;
using namespace web;

class Request {
protected:
	string token;
	Response* response;
public:
	Request() : response(nullptr) {}
	Request(string token) : token(token) {}
	~Request() {
		delete response;
	}
	virtual void execute() = 0;
	Response* getResponse() {
		if (response == nullptr)
			throw;
		return response;
	}
	string getToken() {
		return token;
	}
	bool verifyToken() {
		return (token == "12345");
	}
};

class SearchRequest : public Request {
	string searchKey;
public:
	SearchRequest() {}
	SearchRequest(string token, string searchKey) : Request(token), searchKey(searchKey) {
		
	}
	SearchRequest(json::value json) 
		: SearchRequest(Util::getJSONMember("token", json), Util::getJSONMember("searchKey", json)) {}
	string getSearchKey() {
		return searchKey;
	}
	void execute() {
		SearchResponse s(search(getSearchKey()));
		response = new SearchResponse(s);
	}
};

class DescriptionRequest : public Request {
	string path;
public:
	DescriptionRequest(string token, string path) : Request(token), path(path) {}
	DescriptionRequest(json::value json) :
		DescriptionRequest(Util::getJSONMember("token", json), Util::getJSONMember("path", json)) {}
	string getPath() {
		return path;
	}
	void execute() {
		DescriptionResponse s(getSong(path));
		response = new DescriptionResponse(s);
	}

};

class LyricsRequest : public Request {
	string title;
public:
	LyricsRequest(string token, string title) : Request(token), title(title) {}
	LyricsRequest(json::value json) :
		LyricsRequest(Util::getJSONMember("token", json), Util::getJSONMember("title", json)) {}
	string getTitle() {
		return title;
	}
	void execute() {
		LyricsResponse resp(getLyrics(title));
		response = new LyricsResponse(resp);
	}
};