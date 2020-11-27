#include <iostream>
#include <cpprest/json.h>
#include "Util.cpp"

using namespace std;
using namespace web;

class ClientRequest {
	string token, url;
public:
	ClientRequest(string token, string url) {
		ClientRequest::token = token;
		ClientRequest::url = url;
	}
	ClientRequest(json::value json) {
		token = Util::getJSONMember("token", json);
		url = Util::getJSONMember("url", json);
	}
	string getToken() {
		return token;
	}
	string getURL() {
		return url;
	}
};