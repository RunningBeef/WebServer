#include "include/WebServer.h"
class WebServer;
int main() {
    WebServer webServer;
    webServer.run();
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
