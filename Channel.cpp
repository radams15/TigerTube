//
// Created by rhys on 27/07/2021.
//

#include "Channel.h"

#include <iostream>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "Net.h"

Channel* Channel::new_from_name(std::string name) {
    /*Net::Resp res = Net::get("https://www.youtube.com/c/"+name+"/featured");

    if(res.status_code != 200){
        std::cerr << "FAIL to get channel id URL with error status_code: " << res.status_code << std::endl;
        return nullptr;
    }


    std::regex id_regex(ID_REGEX);

    std::smatch id_match;

    if(std::regex_search(res.text, id_match, id_regex)){
        return new Channel(id_match[1]);
    }

    std::cerr << "FAIL to get channel id regex match" << std::endl;*/

    return NULL;
}

Channel::Channel(std::string id) {
    this->id = id;
    this->name = "";
}

std::string Channel::get_name() {
    /*if(name.empty()) {
        Net::Resp res = Net::get("https://www.youtube.com/feeds/videos.xml?channel_id=" + id);


        if(res.status_code != 200) {
            std::cerr << "FAIL to get channel name URL" << std::endl;
            return "";
        }

        std::regex name_regex(NAME_REGEX);

        std::smatch name_match;

        if(std::regex_search(res.text, name_match, name_regex)) {
            return name_match[1];
        }

        return "";
    }

    return name;*/
    return "TEST";
}

#define STR_EQ(a, b) (!xmlStrcmp(a, (const xmlChar *) b))

void make_video(xmlNode* node, Video* vid, int level){
    for(xmlNode* i=node->children; i!=NULL; i=i->next){
        const xmlChar* key = i->name;

        if(STR_EQ(key, "group")){
            make_video(i, vid, level+1);
        } else if(STR_EQ(key, "link")){
            vid->link = std::string((const char*) xmlGetProp(i, (const xmlChar*)"href"));
        } else if(STR_EQ(key, "published")){
            vid->publish_date = std::string((const char*) xmlNodeGetContent(i));
        } else if(STR_EQ(key, "description")){
            vid->description = std::string((const char*) xmlNodeGetContent(i));
        } else if(STR_EQ(key, "thumbnail")){
            vid->thumbnail = std::string((const char*) xmlGetProp(i, (const xmlChar*)"url"));
        } else if(STR_EQ(key, "title")){
            vid->title = std::string((const char*) xmlNodeGetContent(i));
        }
    }
}

std::vector<Video> Channel::get_vids() {
    std::vector<Video> out;

    Net::Response res = Net::get("https://www.youtube.com/feeds/videos.xml?channel_id=" + id);
    
    xmlDoc* doc = NULL;
    xmlNode* root = NULL;

    if(!(doc = xmlReadMemory(res.content.c_str(), res.content.size(), "", "utf-8", NULL))){
        std::cerr << "error: could not parse data" << std::endl;
        return out;
    }

    root = xmlDocGetRootElement(doc);

    for(root=root->children; root!=NULL; root=root->next){
        if(STR_EQ(root->name, "entry")){
            Video vid;

            make_video(root, &vid, 0);

            out.push_back(vid);
        }
    }

    return out;
}