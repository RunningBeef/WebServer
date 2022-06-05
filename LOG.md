## LOG

### SIGSEGV: Segmentation fault
* debug发现是，给socket命名的ip地址参数自己给的是空指针
* 导致将ip地址转化为网络字节序整数时，出现错误
* 直接使用了htonl(INADDR_ANY)；来做，也就是默认用本地ip地址

### 发现用浏览器连接失败，打印发现每次刷新，web都会以为是一个新的连接请求，每次都重新分配一个socket
* 修改了handleConnection关于ClientSocket对象的部分代码 但是每什么效果，用浏览器还是无法连接到web服务器
* 又猜测可能忘记给这个新的socket增加监事件到epoll里面，一看果然是
* 发现epoll监听的连接socket明明监听到了有客户端的连接事件，但是accept还是返回-1 ? 不清楚原因，不过需要循环accept直到返回-1，因为监听队列里面可能不止有一个的等待连接的socket
* 发现没有成功的把ClientSocket的智能指针复制给httpdata的成员ClientSocket智能指针，导致指向客户端的fd还是-1


### 已经可以接受到起请求报文，但是解析请求报文出现误判,状态机返回Bad Request
* 发现把枚举变量误用成string改了就好了
  
### 用fork让web服务器在后台运行
* 在main函数中实现

### 已经上线到了云服务上，但用WebBench测试时一个请求都不能成功
* 用lsof -i:80 发现大量连接都在time_wait状态
* 没有作好断开连接处理：在httpData中删除对应shared_ptr,关闭fd,删除在epoll中的监听

### 用模板元编程做了个简易的内存池类
**功能**
* 1.扩容 和 压缩
* 2.线程安全
* 3.支持各种类型的分配
* 4.清除归还的内存的内容
* 但是目前不能自动帮有参构造的类作内存池
* 还没测试

### 2022.6.3 
* 用function和shared_ptr<void*>表示定时器类的回调函数和回调函数的参数列表，从而实现定时器类针对不同定时任务的代码复用
* 更新README.md的 定时器设计模块
* 更新Timer.h Timer.cpp WebServer.h WebServer.cpp 关于定时器代码
### 2022.6.5 更新WebBench测压连接0success
* 通过浏览器时能够正常连接服务器获取页面，但是WebBench测压连接0success。猜测可能是连接没有成功关闭
* 用lsof -i:80 发现大量连接都是在close_wait状态说明客户端主动要求断开连接时，服务器没有及时关闭连接
* 进一步考虑时定时器问题，发现定时器默认超时时间过长

### 2022.6.5 段错误
* GDB调试发现WebServer.cpp中cbfunc中event声明了epoll_enent指针但是没有为其申请空间