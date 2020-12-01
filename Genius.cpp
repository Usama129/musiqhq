#pragma once
#include <iostream>
#include <utility>
#include <cpprest/http_client.h>
#include "Response.cpp"
#include "GeniusHit.cpp"

#define TOKEN "insert_genius_token_here"

using namespace std;
using namespace utility::conversions;
using namespace web::http;
using namespace web::http::client;

//Response parseResponse(json::value json) {
//    json::value meta = Util::getJSONMemberAsJSON("meta", json);
//    string status = Util::getJSONMember("status", meta);
//    if (status != "200")
//        return Response(status, {});
//    json::value res = Util::getJSONMemberAsJSON("response", json);
//    json::value hitsJson = Util::getJSONMemberAsJSON("hits", res);
//
//    json::array hitsArray = hitsJson.as_array();
//    int size = hitsArray.size();
//
//    vector<GeniusHit*> hits;
//
//    for (int i = 0; i < size; i++) {
//        GeniusHit* obj = new GeniusHit(hitsArray.at(i));
//        hits.push_back(obj);
//    }
//
//    return Response(status, hits);
//}

json::value search(string searchKey) {

    string token = TOKEN;

    http_client client(U("http://api.genius.com/"));

    //SearchResponse r("-1",{});
    json::value out;

    http_request request(methods::GET);
    request.headers().add(U("Authorization"), to_string_t("Bearer " + token));
    request.headers().add(U("Content-Type"), U("application/x-www-form-urlencoded; charset=UTF-8"));
    utility::string_t PQF = to_string_t("search?q=" + Util::insertSpaceDelimiters(searchKey));
    request.set_request_uri(PQF);
    client.request(request).then([](http_response response)
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
                    out = responseBody;
                    //display_json(, L"R: ");
                }
                catch (http_exception const& e)
                {
                    wcout << e.what() << endl;
                }
            })
            .wait();

            //json::value responseBody = NULL;


            return out;
            //wcout << responseBody.serialize() << endl;
}

json::value getSong(string path) {
    string token = TOKEN;

    http_client client(U("http://api.genius.com/"));

    json::value out;

    http_request request(methods::GET);
    request.headers().add(U("Authorization"), to_string_t("Bearer " + token));
    request.headers().add(U("Content-Type"), U("application/x-www-form-urlencoded; charset=UTF-8"));
    utility::string_t PQF = to_string_t(path + "?text_format=plain");
    request.set_request_uri(PQF);
    client.request(request).then([](http_response response)
        {
            return response.extract_json();
            //return pplx::task_from_result(json::value());
        })
        .then([&](pplx::task<json::value> previousTask)
            {
                try
                {
                    //wcout << previousTask.get().serialize();
                    out = previousTask.get();
                }
                catch (http_exception const& e)
                {
                    wcout << e.what() << endl;
                }
            })
            .wait();

            //json::value responseBody = NULL;


            return out;
            //wcout << responseBody.serialize() << endl;
}

