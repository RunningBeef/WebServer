#include "include/WebServer.h"
class WebServer;
int main() {
    WebServer webServer(4,100,1000,8080);
    //webServer.run();
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
