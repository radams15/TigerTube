//
// Created by rhys on 27/07/2021.
//

#ifndef C_CHANNEL_H
#define C_CHANNEL_H

#include <string>
#include <vector>

#include "Video.h"

class Channel {
private:

    std::string name;

public:
    std::string id;

    explicit Channel(std::string id);

    static Channel* new_from_name(std::string name);

    std::string get_name();

    std::vector<Video> get_vids();
};


#endif //C_CHANNEL_H
