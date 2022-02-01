//
// Created by rhys on 27/07/2021.
//

#ifndef C_SUBSCRIPTIONS_H
#define C_SUBSCRIPTIONS_H

#include "json.hpp"
#include "Channel.h"

#include <string>
#include <vector>

using json=nlohmann::json;

class Config {
private:
    std::string file;

    void load();

public:
    void save();

    std::vector<Channel> subs;
    int quality = 480;

    Config();

    void add_sub(Channel c);

    std::vector<Video> get_vids();

    void remove_sub(std::string id);
};


#endif //C_SUBSCRIPTIONS_H
