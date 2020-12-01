#pragma once

#include <iostream>
#include <cpprest/json.h>
#include <cpprest/http_client.h>
#include <utility>
#include "Util.cpp"

#define TOKEN "insert_audd_token_here"

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace utility::conversions;

json::value getLyrics(string title) {
	http_client client(U("https://api.audd.io/"));
    http_request request(methods::GET);
    request.headers().add(U("Content-Type"), U("multipart/form-data"));
    string token = TOKEN;
    utility::string_t PQF = to_string_t("findLyrics?q=" + Util::insertSpaceDelimiters(title) + "&api_token=" + token);
    request.set_request_uri(PQF);
    json::value out;
    client.request(request).then([](http_response response)
        {
            return response.extract_json();
        })
        .then([&](pplx::task<json::value> previousTask)
            {
                try
                {
                    //wcout << previousTask.get().serialize();
                    json::value responseBody = previousTask.get();
                    out = responseBody;
                    //display_json(, L"R: ");
                }
                catch (http_exception const& e)
                {
                    wcout << e.what() << endl;
                }
            })
            .wait();

            return out;
}