#pragma once

#include <iostream>
#include <cpprest/json.h>
#include "Util.cpp"
#include "Response.cpp"
#include "Genius.h"

using namespace std;
using namespace web;

class ClientRequest {
	string token;
public:
	ClientRequest(string token) : token(token) {}
	virtual Response execute() = 0;
	string getToken() {
		return token;
	}
	bool verifyToken() {
		return (token == "12345");
	}
};

class SearchRequest : public ClientRequest {
	string searchKey;
public:
	SearchRequest(string token, string searchKey) : ClientRequest(token), searchKey(searchKey) {}
	SearchRequest(json::value json) 
		: SearchRequest(Util::getJSONMember("token", json), Util::getJSONMember("searchKey", json)) {}
	string getSearchKey() {
		return searchKey;
	}
	Response execute() {
		cout << "Token is " << getToken() << endl;
		cout << "Search Key is " << getSearchKey() << endl;

		return search(getSearchKey());
	}
};