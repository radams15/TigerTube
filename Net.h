//
// Created by rhys on 21/01/2022.
//

#ifndef YOUTUBECLI_NET_H
#define YOUTUBECLI_NET_H

#include <string>

namespace Net {

    typedef void (*stream_cb)(void*, size_t);

    struct Response{
        int status_code;
        std::string content;
    };

    Response get(std::string url);

    int stream(std::string url, stream_cb stream_func);
}


#endif //YOUTUBECLI_NET_H
