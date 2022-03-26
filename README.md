# WebServer
使用epoll + 非阻塞IO + 边缘触发(ET) + 线程池 实现高并发处理请求<br>
使用EPOLLONESHOT保证一个socket连接在任意时刻都只被一个线程处理 <br>
使用定时器支持HTTP长连接，通过定时器处理超时连接，异常连接<br>
使用 priority queue 实现的最小堆结构管理定时器，使用惰性删除和定期删除，提高性能 <br>
使用状态机解析HTTP请求报文，目前支持 HTTP GET、HEAD方法 <br>
使用RAII手法封装互斥器(pthrea_mutex_t)、 条件变量(pthread_cond_t)等线程同步互斥机制<br>
使用RAII管理文件描述符等 资源使用shared_ptr、weak_ptr管理指针，防止内存泄漏
