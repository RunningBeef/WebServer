//
// Created by yonggeng on 3/25/22.
//
#include "../include/HttpData.h"

HttpData::HttpData()
{
    sharedPtr_httpRequest = std::make_shared<HttpRequest>();
    sharedPtr_httpResponse = std::make_shared<HttpResponse>();
    sharedPtr_clientSocket = std::make_shared<ClientSocket>();
}

void HttpData::setClientSocket(std::shared_ptr<ClientSocket> clientSocket_ptr)
{
    sharedPtr_clientSocket->fd_ = clientSocket_ptr->fd_;
    sharedPtr_clientSocket->epoll_fd_ = clientSocket_ptr->epoll_fd_;

    sharedPtr_clientSocket->clientAddr_ = clientSocket_ptr->clientAddr_;
    sharedPtr_clientSocket->clientAddrlength_ = clientSocket_ptr->clientAddrlength_;
}

HttpData::~HttpData()
{
}

/*socket异常或者超时，调用TimerNode的回调函数，立刻关闭map中fd*/
void HttpData::closeTimerNode()
{
    std::shared_ptr<TimerNode> temp = weakPtr_TimerNode.lock();
    if (temp)
    { /*如果TimerNode对象还存在*/
        std::cout << "close success in mathed closeTimerNode in file " << __FILE__ << " at line " << __LINE__ << std::endl;
        temp->setProcessed();
        temp->cbTask_.function(temp->cbTask_.arg); /*调用回调函数关闭socket*/
        temp.reset();
    }
    else
    {
        std::cout << "timerNode already closed in mathed closeTimerNode in file" << __FILE__ << " at line " << __LINE__ << std::endl;
    }
}

/*socket有新的请求，需要刷新第定时器时使用，断开HttpData和旧的TimerNode的联系*/
void HttpData::breakRelated()
{
    std::shared_ptr<TimerNode> temp = weakPtr_TimerNode.lock();
    if (temp)
    { /*如果TimerNode对象还存在*/
        temp.reset();
    }
    else
    {
        std::cout << "timerNode already closed in mathed breakRelated in file" << __FILE__ << " at line " << __LINE__ << std::endl;
    }
}

/*设置的TimerNode*/
void HttpData::setTimerNode(std::shared_ptr<TimerNode> &timerNode)
{
    weakPtr_TimerNode = timerNode;
}