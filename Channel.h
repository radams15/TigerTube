//
// Created by rhys on 27/07/2021.
//

#ifndef C_CHANNEL_H
#define C_CHANNEL_H

#include <string>
#include <vector>

#include "Video.h"

#define NAME_REGEX "<name>(.*)<\\/name>"
#define ID_REGEX "\\\"externalId\\\":\\\"([0-9a-zA-Z_\\-]*)\\\""

class Channel {
private:

    std::string name;

public:
    std::string id;

    explicit Channel(std::string id);

    static Channel* from_name(std::string name);

    std::string get_name();

    std::vector<Video> get_vids();
};


#endif //C_CHANNEL_H
