#pragma once
#include <iostream>
#include <utility>
#include <cpprest/http_client.h>
#include "Response.cpp"

Response parseResponse(json::value);
Response search(string searchKey);