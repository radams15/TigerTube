//
// Created by rhys on 21/01/2022.
//

#ifndef YOUTUBECLI_NET_H
#define YOUTUBECLI_NET_H

#include <string>

namespace Net {

    struct Response{
        int status_code;
        std::string content;
    };

    Response get(std::string url);

}


#endif //YOUTUBECLI_NET_H
