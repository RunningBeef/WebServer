
cmake_minimum_required(VERSION 3.10)
project(TestMySQL)

set(CMAKE_CXX_STANDARD 11)
set(CXX_FLAGS
        -Wall
        -std=c++11
        -lpthread
        -Wno-unused-parameter
        )

link_libraries(pthread)
link_libraries(m)
link_libraries(mysqlclient)
link_directories(../json/lib)
link_libraries(jsoncpp)
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_COMPILER "g++")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")


set(SOURCE_FILES
       testMySQL.cc
       MySQLConn.cc
       ConnectionPool.cc
       ../locker/Mutex.cc
       ../locker/Condition.cc
        )

# src/Socket.cpp src/ThreadPool.cpp src/HttpRequest.cpp src/HttpResponse.cpp src/ParseHttpRequest.cpp src/ParseHttpResponse.cpp src/HttpData.cpp src/Timer.cpp src/Epoll.cpp src/WebServer.cpp src/Condition.cpp src/Mutex.cpp main.cpp
add_executable(TestMySQL  ${SOURCE_FILES})#生成可执行文件