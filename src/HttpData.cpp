//
// Created by yonggeng on 3/25/22.
//
#include "../include/HttpData.h"

HttpData::HttpData(ClientSocket & clientSocket){
    sharedPtr_httpRequest = std::make_shared<HttpRequest>();
    sharedPtr_httpResponse = std::make_shared<HttpResponse>();
    sharedPtr_clientSocket = std::make_shared<ClientSocket>(clientSocket);
    epoll_fd_ = sharedPtr_clientSocket->epoll_fd_;

}

/*socket异常或者超时，调用TimerNode的回调函数，立刻关闭map中fd*/
/*socket更新时，不方便从堆中直接找出这个定时器删除，
 * 而是先断开HttpData和旧的TimerNode的联系
 * 再给这个SOCKET建立新的TimerNode*/

void HttpData::closeTimerNode()
{

    std::shared_ptr<TimerNode> temp = weakPtr_TimerNode.lock();
    if(temp){/*如果TimerNode对象还存在*/
        temp->setDeleted();
        temp->cbFunc_(temp->httpData_);/*调用回调函数关闭socket*/
        temp.reset();
    }
}

/*断开HttpData和旧的TimerNode的联系*/
void HttpData::breakRelated(){
    std::shared_ptr<TimerNode> temp = weakPtr_TimerNode.lock();
    if(temp){/*如果TimerNode对象还存在*/
        temp.reset();
    }
}

/*设置的TimerNode*/
void HttpData::setTimerNode(std::shared_ptr<TimerNode> & timerNode){
    weakPtr_TimerNode = timerNode;
}