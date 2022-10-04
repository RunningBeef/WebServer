//
// Created by yonggeng on 3/24/22.
//

#ifndef RUNNINGBEEF_WEBSERVER_UTIL_H
#define RUNNINGBEEF_WEBSERVER_UTIL_H

// #pragma once


#include <string>

/*移除多余 " \t" */
std::string& ltrim(std::string &str) {
    if (str.empty()) {
        return str;
    }

    str.erase(0, str.find_first_not_of(" \t"));
    return str;
}

std::string& rtrim(std::string &str) {
    if (str.empty()) {
        return str;
    }
    str.erase(str.find_last_not_of(" \t")+1);
    return str;
}

std::string& trim(std::string& str) {
    if (str.empty()) {
        return str;
    }

    ltrim(str);
    rtrim(str);
    return str;
}


#endif
