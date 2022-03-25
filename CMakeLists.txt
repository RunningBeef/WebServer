cmake_minimum_required(VERSION 3.21)
project(RunningBeef_WebServer)

set(CMAKE_CXX_STANDARD 14)

add_executable(RunningBeef_WebServer main.cpp include/Socket.h src/Socket.cpp include/Noncopyable.h include/Mutex.h src/Mutex.cpp include/Condition.h src/Condition.cpp include/HttpRequest.h src/HttpRequest.cpp include/ParseHttpRequest.h src/ParseHttpRequest.cpp include/Util.h include/HttpData.h include/HttpResponse.h src/HttpResponse.cpp include/Timer.h src/Timer.cpp src/HttpData.cpp)
