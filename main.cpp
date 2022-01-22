#include <iostream>
#include "Channel.h"
#include "Subscriptions.h"

int main() {
    Subscriptions subs;

    std::vector<Video> vids = subs.get_vids();

    for(int i=0 ; i<vids.size() ; i++) {
        Video video = vids[i];

        std::cout << video.title << " => " << video.link << "\n";
    }


    subs.save();
    return 0;
}
