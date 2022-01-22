//
// Created by rhys on 27/07/2021.
//

#include "Video.h"

#include <cstdio>
#include <iostream>
#include <memory>

#include "json.hpp"

using json = nlohmann::json;

std::string exec(std::string cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}


std::vector<Format> Video::get_formats() {
    return get_formats(link);
}

std::vector<Format> Video::get_formats(std::string link) {
    std::vector<Format> out;

    std::string output = exec("yt-dlp -j " + link);

    json info = json::parse(output);

    for(int i=0 ; i<info["formats"].size() ; i++){
        json form = info["formats"][i];
        std::string acodec = form["acodec"];
        std::string vcodec = form["vcodec"];

        if(acodec == "none" or vcodec == "none" or vcodec == "mp4v.20.3"){
            continue;
        }

        out.push_back((Format){form["url"], form["format_note"]});
    }

    return out;
}
