//
// Created by yonggeng on 3/23/22.
//

//#ifndef RUNNINGBEEF_WEBSERVER_NONCOPYABLE_H
//#define RUNNINGBEEF_WEBSERVER_NONCOPYABLE_H
#pragma once

class Noncopyable
{
public:
    Noncopyable(const Noncopyable & ) = delete;
    Noncopyable operator = (const Noncopyable &) = delete;

protected:
    Noncopyable() = default;
    ~Noncopyable() = default;
};
//#endif //RUNNINGBEEF_WEBSERVER_NONCOPYABLE_H
