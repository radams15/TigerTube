//
// Created by rhys on 21/01/2022.
//

#include "Net.h"

#include <curl/curl.h>

#include <iostream>

static size_t string_write_cb(void *contents, size_t size, size_t nmemb, void *userp){
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

static size_t stream_write_cb(void *contents, size_t size, size_t nmemb, void *userp){
    size_t len = size * nmemb;
    ((Net::stream_cb) userp)(contents, len);

    return len;
}

Net::Response Net::get(std::string url) {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    Response out;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, string_write_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &(out.content));
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &(out.status_code));
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        return out;
    }
    return (Response){-1, ""};
}

int Net::stream(std::string url, Net::stream_cb stream_func){
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    int out;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, stream_write_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) stream_func);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &(out));
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        return out;
    }

    return -1;
}