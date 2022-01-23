//
// Created by rhys on 27/07/2021.
//

#include "Config.h"
#include "Fs.h"

#include <fstream>
#include <iostream>

#include <sys/types.h>
#include <pwd.h>

#ifdef __linux
#define CONF_DIR "/home/rhys/.config/"
#elif defined(__APPLE__)
#define CONF_DIR "/Users/rhys/Library/"
#endif


Config::Config() {
    this->file = std::string(CONF_DIR) + "yt_saves.json";
    this->subs.clear();

    if(Fs::exists(file)){
        load();
    }
}

void Config::load() {
    std::ifstream in(file);

    json j;

    in >> j;

    for(json id : j["subs"]){
        Channel c(id.get<std::string>());
        subs.push_back(c);
    }

    try{
       quality = j["quality"].get<int>();
    }catch(std::exception& e){
        std::cerr << "No default quality, setting to 480" << std::endl;
    }
}

void Config::save() {
    std::ofstream out(file);

    json j = json::object();

    json sub_list = json::array();

    for(Channel c : subs){
        sub_list.push_back(c.id);
    }

    j["subs"] = sub_list;
    j["quality"] = quality;

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

std::vector<Video> Config::get_vids() {
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
        return a.publish_date > b.publish_date;
    });

    return out;
}

void Config::remove_sub(std::string id) {
    auto it = std::remove_if(subs.begin(), subs.end(), [id](Channel chan){
        return chan.id == id;
    });

    subs.erase(it, subs.end());
}

void Config::add_sub(Channel c) {
    subs.push_back(c);
}
