#include <iostream>
#include <cpprest/json.h>
#include <utility>

using namespace std;
using namespace web;
using namespace utility::conversions;

class Util {
public:
	static string getJSONMember(string key, json::value json) {
		utility::string_t t_key = to_string_t(key);
		json::value j = json.at(t_key);
		return to_utf8string(j.serialize());
	}
};