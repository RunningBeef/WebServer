#include "include/WebServer.h"
class WebServer;
bool daemonize()
{
    int pid = fork();
    if (pid < 0)
    {
        std::cout << "fork file" << std::endl;
        return false;
    }
    if (pid > 0)
    {
        exit(0);
    }
    /*设置文件权限掩码。当进程创建新文件(使用open(const char*pathname,int flags,mode_t mode)系统调用)时, 文件的权限将是mode&0777*/
    umask(0);
    /* 创建新会话，将本进程作为进程组的首领 */
    pid_t sid = setsid();
    if (sid < 0)
    {
        return false;
    }
    /* 关闭标准输入、输出、错误设备 */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    /* 将标准输入、输出、错误设备重定向到到/dev/null文件 */
    open("/dev/null", O_RDONLY);
    open("/dev/null", O_WRONLY);
    open("/dev/null", O_WRONLY);
    return true;
}

void test()
{
    WebServer webServer("192.168.199.128", 7777, 4, 1024, 1000);
    webServer.run();
    std::cout << "WebServer over!" << std::endl;
}
int main(int argc, char **argv)
{
    // test();

    if (argc < 7)
    {
        std::cout << "usage:" << argv[0] << " ip_address port_number thread_number maxTask_number waitTime flag_run_in_backstage(1\0)" << std::endl;
        return 1;
    }

    if (argv[6][0] == '1')
    {
        daemonize();
    }
    char *ip = argv[1];
    int port = atoi(argv[2]), threadNum = atoi(argv[3]), maxTask = atoi(argv[4]), blockTime = atoi(argv[5]);
    WebServer webServer(ip, port, threadNum, maxTask, blockTime);
    webServer.run();
    std::cout << "WebServer over!" << std::endl;
    return 0;
}
