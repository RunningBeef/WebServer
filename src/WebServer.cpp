//
// Created by yonggeng on 3/26/22.
//

#include "../include/WebServer.h"
std::string basePath = "/";

void cbfunc(WebServer * webserver,std::shared_ptr<HttpData> httpData){
    if(httpData){
        struct epoll_event * event;
        event->events = Epoll::DEFAULT_EVENT;
        int socket_fd = event->data.fd = httpData->sharedPtr_clientSocket->fd_;
        webserver->epoll_->modFd(EPOLL_CTL_DEL,socket_fd,event);

        auto it = webserver->httpDataMap_.find(socket_fd);
        if(it == webserver->httpDataMap_.end()){
            std::cout << "error : httpData already erase !!! "
                      << " in file " << __FILE__ << " at line " << __LINE__ << std::endl;
        }
        webserver->httpDataMap_.erase(it);
        /*fd会在clientSocket析构函数中关闭*/
    }

}


WebServer::WebServer(int threadNum,int maxTask, int eventSize,int port, char * ip)
: serverSocket_(port,ip),threadNum_(threadNum),maxTask_(maxTask),eventSize_(eventSize),epoll_(new Epoll(eventSize)),timerManager(new TimerManager){

    threadPool_ = std::make_shared<ThreadPool>(threadNum_,maxTask_);
    /*给serverSocket设置监听*/
    serverSocket_.epoll_fd_ = epoll_->get_epoll_fd();
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = serverSocket_.socket_fd_;
    epoll_->modFd(EPOLL_CTL_ADD,serverSocket_.socket_fd_,&event);
}

WebServer::~WebServer() {
    if(timerManager){
        delete timerManager;
    }else{
        std::cout << " * epoll error in " << __FILE__ << " at line " << __LINE__ << std :: endl;
    }

    if(epoll_)
    {
        delete epoll_;
    }else{
        std::cout << " * epoll error in " << __FILE__ << " at line " << __LINE__ << std :: endl;

    }
    /*删除serverSocket设置监听*/
    serverSocket_.epoll_fd_ = epoll_->get_epoll_fd();
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = serverSocket_.socket_fd_;
    epoll_->modFd(EPOLL_CTL_DEL,serverSocket_.socket_fd_,&event);
}

void ::WebServer::handleConnection(){
    /*因为ET模式，记得把socket设置为非阻塞的*/
    ClientSocket clientSocket;
    serverSocket_.accept(clientSocket);
    std::shared_ptr<HttpData> httpData = std::make_shared<HttpData>(clientSocket);
    httpData->epoll_fd_ = epoll_->get_epoll_fd();/*注意传递epoll_fd*/
    size_t interval;
    std::shared_ptr<TimerNode> timerNode = std::make_shared<TimerNode>(interval,httpData,this,cbfunc) ;
    timerManager->addTimerNode(timerNode);
}


std::vector<std::shared_ptr<HttpData>> WebServer::handleEvents() {
    int ret = epoll_->wait(0);
    std::vector<std::shared_ptr<HttpData>> epollInSocket;
    if(ret == -1){
        std::cout << "epoll_wait error: " << errno << " in file " << __FILE__
                  << "at line " << __LINE__ << std:: endl;
    }
    struct epoll_event * events = epoll_->getEpollEvent();
    for(int i = 0; i < ret; ++i){
        int fd = events[i].data.fd;
        /*1 有新的连接到来*/
        if(fd == serverSocket_.socket_fd_){
            handleConnection();
        }else{

            /*2socket异常 移除定时器（懒惰标记），关闭文件描述符
             *EPOLLERR 错误  EPOLLRDHUP TCP连接被对方关闭 EPOLLHUP 管道写段关闭*/
            if( (events[i].events & EPOLLERR) || (events[i].events & EPOLLRDHUP) || (events[i].events & EPOLLHUP)){
                auto it = httpDataMap_.find(fd);
                if(it != httpDataMap_.end()){
                    it->second->closeTimerNode();/*内部用定时器回调函数，关闭*/
                }else{
                    std::cout << "error: 并发异常，httpdataMap不在，或者被提前删除 "
                    << "at file " << __FILE__ << " at line " << __LINE__ << std::endl;
                }
            }
            else{/*3.socket有数据可读*/
                auto it = httpDataMap_.find(fd);
                if(it != httpDataMap_.end()){
                    /*数据 或带外数据*/
                    if((events[i].events & EPOLLIN) || (events[i].events & EPOLLPRI)){
                        it->second->breakRelated();/*断开和原httpData连接*/
                        epollInSocket.push_back(it->second);
                    }
                    else{
                        std::cout << "error::其他事件发生 "
                                  << "at file " << __FILE__ << " at line " << __LINE__ << std::endl;
                    }
                }else{
                    std::cout << "error::httpData资源已经被移除"
                               << "at file " << __FILE__ << " at line " << __LINE__ << std::endl;
                }
            }
        }
    }
    return epollInSocket;
}

void WebServer::do_request(std::shared_ptr<void> arg) {
    std::shared_ptr<HttpData> sharedHttpData = std::static_pointer_cast<HttpData>(arg);//向下转化

    char buffer[BUFFERSIZE];

    bzero(buffer, BUFFERSIZE);
    int check_index = 0, read_index = 0, next_line = 0;
    ssize_t recv_data;
    ParseHttpRequest::PARSE_STATUS  parse_status = ParseHttpRequest::PARSE_REQUESTLINE;

    while (true) {

        // FIXME 这里也是同样的，由于是非阻塞IO，所以返回-1 也不一定是错误，还需判断error
        recv_data = recv(sharedHttpData->sharedPtr_clientSocket->fd_, buffer + read_index, BUFFERSIZE - read_index, 0);
        if (recv_data == -1) {
            if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                return;  // FIXME 请求不完整该怎么办，继续加定时器吗？还是直接关闭
            }
            std::cout << "reading faild" << std::endl;
            return;
        }
        // todo 返回值为 0对端关闭, 这边也应该关闭定时器

        if (recv_data == 0) {
            std::cout << "connection closed by peer" << std::endl;
            break;
        }
        read_index += recv_data;

        ParseHttpRequest::HTTP_CODE  retcode = ParseHttpRequest::pare_httpRequest(
                buffer, check_index, read_index, next_line,parse_status,*sharedHttpData->sharedPtr_httpRequest);

        if (retcode == ParseHttpRequest::NO_REQUEST) {
            continue;
        }


        if (retcode == ParseHttpRequest::GET_REQUEST) {
            // FIXME 检查 keep_alive选项
            auto it = sharedHttpData->sharedPtr_httpRequest->headerToStringMap_.find(HttpRequest::Connection);
            if (it != sharedHttpData->sharedPtr_httpRequest->headerToStringMap_.end()) {
                if (it->second == "keep-alive") {
                    sharedHttpData->sharedPtr_httpResponse->setKeepAlive(true);
                    // timeout=20s
                    sharedHttpData->sharedPtr_httpResponse->addHeader("Keep-Alive", std::string("timeout=20"));
                } else {
                    sharedHttpData->sharedPtr_httpResponse->setKeepAlive(false);
                }
            }
            ParseHttpResponse::header(sharedHttpData);
            ParseHttpResponse::getMime(sharedHttpData);
            // FIXME 之前测试时写死的了文件路径导致上服务器出错
            //static_file(sharedHttpData, "/Users/lichunlin/CLionProjects/webserver/version_0.1");
            ParseHttpResponse::FILE_STATUS  fileStatus = ParseHttpResponse::get_file(sharedHttpData, basePath.c_str());
            ParseHttpResponse::send(sharedHttpData, fileStatus);
            // 如果是keep_alive else sharedHttpData将会自动析构释放clientSocket，从而关闭资源
            if (sharedHttpData->sharedPtr_httpResponse->keep_alive()) {
                //FIXME std::cout << "再次添加定时器  keep_alive: " << sharedHttpData->clientSocket_->fd << std::endl;
                struct epoll_event event;
                event.events = Epoll::DEFAULT_EVENT;
                int fd = event.data.fd = sharedHttpData->sharedPtr_clientSocket->fd_;

                epoll_->modFd(EPOLL_CTL_ADD, fd,&event);
                std::shared_ptr<TimerNode> timerNode = std::make_shared<TimerNode>(TimerNode::DEFAULT_INTERVAL_SEC,sharedHttpData,this,cbfunc) ;
                timerManager->addTimerNode(timerNode);
            }

        } else {
            // todo Bad Request 应该关闭定时器,(其实定时器已经关闭,在每接到一个新的数据时)
            std::cout << "Bad Request" << std::endl;
        }
    }
}

void WebServer::run() {

    std::vector<std::shared_ptr<HttpData>> epollInSocket;

    while(true){
        epollInSocket = handleEvents();
        for(auto &it : epollInSocket){
            Task task;
            task.function = std::bind(&WebServer::do_request, this, std::placeholders::_1);
            task.arg = it;
            threadPool_->appendTask(task);
        }
        /*每次处理完事件后就检查定时器*/
        timerManager->tick();

    }
}