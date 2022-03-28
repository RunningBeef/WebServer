//
// Created by yonggeng on 3/25/22.
//

#include "../include/HttpResponse.h"

std::unordered_map<std::string, MimeType> Mime_map = {
        {".html", "text/html"},
        {".xml", "text/xml"},
        {".xhtml", "application/xhtml+xml"},
        {".txt", "text/plain"},
        {".rtf", "application/rtf"},
        {".pdf", "application/pdf"},
        {".word", "application/msword"},
        {".png", "image/png"},
        {".gif", "image/gif"},
        {".jpg", "image/jpeg"},
        {".jpeg", "image/jpeg"},
        {".au", "audio/basic"},
        {".mpeg", "video/mpeg"},
        {".mpg", "video/mpeg"},
        {".avi", "video/x-msvideo"},
        {".gz", "application/x-gzip"},
        {".tar", "application/x-tar"},
        {".css", "text/css"},
        {"", "text/plain"},
        {"default","text/plain"}
};

void HttpResponse::appendBuffer(char *buffer) const{

    // 版本
    sprintf(buffer, "%s %d %s\r\n", mVersion,mStatusCode, mStatusMsg.c_str());

    // 头部字段
    for (auto it = mHeaders.begin(); it != mHeaders.end(); it++) {
        sprintf(buffer, "%s%s: %s\r\n", buffer, it->first.c_str(), it->second.c_str());
    }
    sprintf(buffer, "%sContent-type: %s\r\n", buffer, mMime.type.c_str());
    // keep_alive
    if (keep_alive_) {
        sprintf(buffer, "%sConnection: keep-alive\r\n", buffer);
    } else {
        sprintf(buffer, "%sConnection: close\r\n", buffer);
    }
}





