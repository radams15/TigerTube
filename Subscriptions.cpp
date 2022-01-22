//
// Created by rhys on 27/07/2021.
//

#include "Subscriptions.h"
#include "Fs.h"

#include <fstream>
#include <iostream>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#define CONF_DIR "/home/rhys/.config/"


Subscriptions::Subscriptions() {
    this->file = std::string(CONF_DIR) + "yt_saves.json";
    this->subs.clear();

    if(Fs::exists(file)){
        load();
    }
}

void Subscriptions::load() {
    std::ifstream in(file);

    json j;

    in >> j;

    for(json id : j){
        Channel c(id.get<std::string>());
        subs.push_back(c);
    }
}

void Subscriptions::save() {
    std::ofstream out(file);

    json j = json::array();

    for(Channel c : subs){
        j.push_back(c.id);
    }

    out << std::setw(4) << j << std::endl;
}


typedef struct {
    Channel c;
    std::vector<Video>* out;
} VidData;

static pthread_mutex_t mutex;

void* get_vid(void* ptr){
    auto* v = (VidData*) ptr;

    pthread_mutex_lock(&mutex);

    auto c_vids = v->c.get_vids();

    v->out->insert(v->out->end(), c_vids.begin(), c_vids.end());
    pthread_mutex_unlock(&mutex);

    free(ptr);

    pthread_exit(NULL);
}

std::vector<Video> Subscriptions::get_vids() {
    std::vector<Video> out;

    std::vector<pthread_t> threads;

    for(const Channel& c : subs){
        threads.push_back(0);

        VidData* v = new VidData{c, &out};

        pthread_create(&threads[threads.size()-1], NULL, get_vid, v);
    }

    for(auto t : threads){
        void* status;
        pthread_join(t, &status);
    }


    sort(out.begin(), out.end(), [](Video a, Video b) -> bool{
        return a.publish_date < b.publish_date;
    });

    return out;
}

void Subscriptions::remove(std::string id) {
    auto it = std::remove_if(subs.begin(), subs.end(), [id](Channel chan){
        return chan.id == id;
    });

    subs.erase(it, subs.end());
}
