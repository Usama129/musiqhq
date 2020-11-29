#pragma once
#include <iostream>
#include <utility>
#include <cpprest/http_client.h>
#include "Response.cpp"

using namespace std;
using namespace utility::conversions;
using namespace web::http;
using namespace web::http::client;

Response parseResponse(json::value json) {
    json::value meta = Util::getJSONMemberAsJSON("meta", json);
    string status = Util::getJSONMember("status", meta);
    if (status != "200")
        return Response(status, {});
    json::value res = Util::getJSONMemberAsJSON("response", json);
    json::value hitsJson = Util::getJSONMemberAsJSON("hits", res);

    json::array hitsArray = hitsJson.as_array();
    int size = hitsArray.size();

    vector<Hit*> hits;

    for (int i = 0; i < size; i++) {
        GeniusHit* obj = new GeniusHit(hitsArray.at(i));
        hits.push_back(obj);
    }

    return Response(status, hits);
}

Response search(string searchKey) {

    http_client client(U("http://api.genius.com/"));

    Response r("-1",{});

    utility::string_t PQF = to_string_t("search?q=" + Util::insertSpaceDelimiters(searchKey)
        + "&access_token=8-KsE8pP3OIPlV5v6p80SSsR8Z_iKtuRQZpBgApzw5gk1-di9jOygUaNeWFSbn-v");

    client.request(methods::GET, PQF).then([](http_response response)
        {
            return response.extract_json();
            //return pplx::task_from_result(json::value());
        })
        .then([&](pplx::task<json::value> previousTask)
            {
                try
                {
                    //wcout << previousTask.get().serialize();
                    json::value responseBody = previousTask.get();
                    r = parseResponse(responseBody);
                    //display_json(, L"R: ");
                }
                catch (http_exception const& e)
                {
                    wcout << e.what() << endl;
                }
            })
            .wait();

            //json::value responseBody = NULL;


            return r;
            //wcout << responseBody.serialize() << endl;
}
