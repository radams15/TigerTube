//
// Created by rhys on 22/01/2022.
//

#include "Fs.h"

#include <sys/stat.h>

bool Fs::exists(const std::string& dir) {
    struct stat sb{};
    if (stat(dir.c_str(), &sb) == 0){
        return true;
    }
    return false;
}
