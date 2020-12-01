#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include "Request.cpp"
#include "Genius.h"

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
using namespace std;


void handle_get(http_request httpRequest)
{
    wcout << "\nGET request processed for " << httpRequest.remote_address() << endl;
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

    Request* request = nullptr;
    
    // Search Request
    if (absURL == "/musiqhq/search") {
        
        try {
            request = new SearchRequest(requestBody);
        }
        catch (exception& e) {
            cout << e.what();
            answer[U("bad_search")] = json::value(to_string_t("Check token and search key"));
            httpRequest.reply(status_codes::BadRequest, answer);
            return;
        }

    }

    // Song description Request
    if (absURL == "/musiqhq/song/description") {
        try {
            request = new DescriptionRequest(requestBody);
        }
        catch (exception& e) {
            cout << e.what();
            answer[U("bad_song_request")] = json::value(to_string_t("Check token and song path"));
            httpRequest.reply(status_codes::BadRequest, answer);
            return;
        }
    }

    //Lyrics
    if (absURL == "/musiqhq/song/lyrics") {
        try {
            request = new LyricsRequest(requestBody);
        }
        catch (exception& e) {
            cout << e.what();
            answer[U("bad_lyrics_request")] = json::value(to_string_t("Check token and title"));
            httpRequest.reply(status_codes::BadRequest, answer);
            return;
        }
    }

    if (request == nullptr) {
        answer[U("bad_request")] = json::value(to_string_t("Server did not find a matching endpoint"));
        httpRequest.reply(status_codes::BadRequest, answer);
        return;
    }

    if (request->verifyToken()) {
        request->execute();
        answer = request->getResponse()->to_json();
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
            .then([&listener]() {wcout << "Listening at " << listener.uri().to_string(); })
            .wait();

        while (true);
    }
    catch (exception const& e)
    {
        wcout << e.what() << endl;
    }

    return 0;
}