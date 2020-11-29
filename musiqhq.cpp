#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include "ClientRequest.cpp"
#include "Genius.h"

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
using namespace std;


void handle_get(http_request httpRequest)
{
    cout << "\nGET\n";

    auto answer = json::value::object();

    json::value requestBody = NULL;

    
    string absURL = to_utf8string(httpRequest.absolute_uri().to_string());

    try {
        requestBody = httpRequest.extract_json().get();
    }
    catch (exception& e) {
        cout << e.what();
    }

    if (requestBody.is_null()) { 
        answer[U("bad_request")] = json::value(to_string_t("Request body cannot be empty"));
        httpRequest.reply(status_codes::BadRequest, answer);
        return;
    }

    ClientRequest* request = nullptr;
    
    // Search Request
    if (absURL == "/musiqhq/search") {
        request = new SearchRequest(requestBody);

    }

    // Lyrics Request
    //if (absURL == "/musiqhq/lyrics")

    if (request == nullptr) {
        answer[U("bad_request")] = json::value(to_string_t("Server did not find a matching endpoint"));
        httpRequest.reply(status_codes::BadRequest, answer);
        return;
    }

    if (request->verifyToken()) {
        answer = request->execute().to_json();
        httpRequest.reply(status_codes::OK, answer);
        delete request;
        return;
    }
    else {
        answer[U("bad_request")] = json::value(to_string_t("Invalid Token"));
        httpRequest.reply(status_codes::BadRequest, answer);
        delete request;
        return;
    }

    //Final
    answer[U("bad_request")] = json::value(to_string_t("Server did not find a matching endpoint"));
    httpRequest.reply(status_codes::BadRequest, answer);

    delete request;
}

int main()
{
    http_listener listener(L"http://localhost/musiqhq");


    listener.support(methods::GET, handle_get);
 

    try
    {
        listener
            .open()
            .then([&listener]() {cout << "Listening!\n"; })
            .wait();

        while (true);
    }
    catch (exception const& e)
    {
        wcout << e.what() << endl;
    }

    return 0;
}