//
// Created by rhys on 27/07/2021.
//

#include "Channel.h"

#include <iostream>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <Regex.hpp>

#include "Net.h"

Channel* Channel::from_name(std::string name) {
    Net::Response res = Net::get("https://www.youtube.com/c/"+name+"/featured");

    /*if(res.status_code != 200){
        std::cerr << "FAIL to get channel id URL with error status_code: " << res.status_code << std::endl;
        return nullptr;
    }*/


    ft::Regex id_regex(ID_REGEX);

    ft::Regex::result_t id_match;

    if(id_regex.match(res.content, id_match)){
        return new Channel(id_match.groups[1]);
    }

    std::cerr << "FAIL to get channel id regex match" << std::endl;

    return NULL;
}

Channel::Channel(std::string id) {
    this->id = id;
    name = "";
}

std::string Channel::get_name() {
    if(name.empty()) {
        Net::Response res = Net::get("https://www.youtube.com/feeds/videos.xml?channel_id=" + id);

        /*if(res.status_code != 200) {
            std::cerr << "FAIL to get channel name URL" << std::endl;
            return "";
        }*/


        ft::Regex name_regex(NAME_REGEX);

        ft::Regex::result_t match;
        if(!name_regex.match(res.content, match)) {
            return "";
        }

        name = match.groups[1];
    }

    return name;
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
        }else if(STR_EQ(key, "title")){
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

            vid.channel_name = get_name();
            vid.channel_id = id;

            make_video(root, &vid, 0);

            out.push_back(vid);
        }
    }

    return out;
}