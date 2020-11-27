#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include "ClientRequest.cpp"

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
using namespace std;

void handle_get(http_request httpRequest)
{
    cout << "\nGET\n";

    auto answer = json::value::object();

    json::value requestBody = NULL;

    try {
        requestBody = httpRequest.extract_json().get();
    }
    catch (exception& e) {
        cout << e.what();
    }

    if (requestBody == NULL) { // TODO: Add appropriate error message to answer obj
        httpRequest.reply(status_codes::BadRequest, answer);
        return;
    }

    ClientRequest clientRequest(requestBody);

    cout << "Token is " << clientRequest.getToken() << endl;
    cout << "URL is " << clientRequest.getURL() << endl;

    httpRequest.reply(status_codes::OK, answer);
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