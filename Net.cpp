//
// Created by rhys on 21/01/2022.
//

#include "Net.h"

#include <curl/curl.h>

static size_t string_write_cb(void *contents, size_t size, size_t nmemb, void *userp){
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

Net::Response Net::get(std::string url) {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    Response out;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, string_write_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &(out.content));
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &(out.status_code));
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        return out;
    }
    return (Response){-1, ""};
}

std::string Net::get_redirect(std::string url) {
    CURL *curl;
    CURLcode res;

    long response_code;
    char* location;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        /* example.com is redirected, figure out the redirection! */

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
            return "";
        } else {
            res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            if ((res == CURLE_OK) && ((response_code / 100) != 3)) {
                fprintf(stderr, "Not a redirect.\n");

                return url;
            } else {
                res = curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &location);

                if ((res == CURLE_OK) && location) {
                    printf("Redirected to: %s\n", location);

                    return std::string(location);
                }
            }
        }
        curl_easy_cleanup(curl);
    }


    return "";
}
