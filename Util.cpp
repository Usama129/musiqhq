#include <iostream>
#include <cpprest/json.h>
#include <utility>
#include <regex>
#pragma once
using namespace std;
using namespace web;
using namespace utility::conversions;

class Util {

public:
	static string getJSONMember(string key, json::value json) {
		utility::string_t t_key = to_string_t(key);
		json::value j = json.at(t_key);
		string s = to_utf8string(j.serialize());
		s.erase(remove(s.begin(), s.end(), '\"'), s.end()); // removing quote marks from final string
		return s;
	}

	static json::value getJSONMemberAsJSON(string key, json::value json) {
		utility::string_t t_key = to_string_t(key);
		json::value j = json.at(t_key);
		return j;
	}

	static string insertSpaceDelimiters(string text)
	{
		regex space("[[:space:]]");
		return regex_replace(text, space, "%20");
	}
};