//
// Created by yonggeng on 3/25/22.
//

//#ifndef RUNNINGBEEF_WEBSERVER_HTTPRESPONSE_H
//#define RUNNINGBEEF_WEBSERVER_HTTPRESPONSE_H
#pragma once

#include <iostream>
#include <string>
#include "HttpRequest.h"

struct MimeType {
    MimeType(const std::string &str) : type(str) {};

    MimeType(const char *str) : type(str) {};

    std::string type;
};

/*媒体资源类型*/
extern std::unordered_map<std::string, MimeType> Mime_map;

class HttpResponse {

public:
    enum HttpStatusCode {
        Unknow,
        k200Ok = 200,
        k403forbiden = 403,
        k404NotFound = 404
    };

    explicit HttpResponse(bool mkeep = true)
            : mStatusCode(Unknow), keep_alive_(mkeep), mMime("text/html"), mBody(nullptr),
              mVersion(HttpRequest::HTTP1_1) {}

    void setStatusCode(HttpStatusCode code) {
        mStatusCode = code;
    }

    void setBody(const char *buf) {
        mBody = buf;
    }

    void setContentLength(int len) {
        mContentLength = len;
    }

    void setVersion(const HttpRequest::HTTP_VERSION &version) {
        mVersion = version;
    }

    void setStatusMsg(const std::string &msg) {
        mStatusMsg = msg;
    }

    void setFilePath(const std::string &path) {
        mFilePath = path;
    }

    void setMime(const MimeType &mime) {
        mMime = mime;
    }

    void setKeepAlive(bool isalive) {
        keep_alive_ = isalive;
    }

    void addHeader(const std::string &key, const std::string &value) {
        mHeaders[key] = value;
    }

    bool keep_alive() const {
        return keep_alive_;
    }

    const HttpRequest::HTTP_VERSION version() const {
        return mVersion;
    }

    const std::string &filePath() const {
        return mFilePath;
    }

    HttpStatusCode statusCode() const {
        return mStatusCode;
    }

    const std::string &statusMsg() const {
        return mStatusMsg;
    }

    void appendBuffer(char *) const;

    ~HttpResponse() {
        if (mBody != nullptr)
            delete[] mBody;
    }

private:
    HttpStatusCode mStatusCode;/*状态码*/
    HttpRequest::HTTP_VERSION mVersion;
    std::string mStatusMsg;/*状态原因短语*/
    bool keep_alive_;/*长链接*/
    MimeType mMime;/*媒体类型*/
    const char *mBody;/*返回给客户端的媒体信息*/
    int mContentLength;
    std::string mFilePath;
    std::unordered_map<std::string, std::string> mHeaders;
};
//
//#endif //RUNNINGBEEF_WEBSERVER_HTTPRESPONSE_H
