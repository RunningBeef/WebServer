## 保姆教程，从0开始编写、运行、调试这个项目
* 这其实算是我做的第一个多文件的c++项目，所以很多东西都不会，都是从0开始

### 开发部署环境
* 操作系统: Ubuntu

* 编译器: g++

* 版本控制: git

* 自动化构建: cmake

* 集成开发工具: vscode
  
* 压测工具：WebBench

### 学习之路
* 前置知识：这个项目实际上是 [linux高性能服务器编程][1],这本书里面知识的实践，看完书的1、2部分就可以搭环境撸代码了，第3部分优化与监测可以项目搭建好后看。
  
* Ubuntu、g++、git、网上搜索博客自行配置即可，git在要把项目上传到github上才会用到,用的使用找个博客了解下就行，属于现用现学
  
* **[vscode,cmake配置实现c/c++开发教程推荐][2]，讲的很好**
[1]:https://read.douban.com/reader/ebook/15233070/
[2]:https://www.bilibili.com/video/BV1fy4y1b7TC?share_source=copy_web 

### 实现之路
* 设计项目整体结构，自己分别要实现哪些模块，如线程池、内存池、定时器、http请求的解析和响应。这些模块都可以专门写个头文件，并且用.cpp文件实现对应的方法，对于互斥锁、条件变量、socket、epoll、的使用也可也这样。琐细的实现可以交给封装好的类来实现，主函数只要负责程序逻辑即可

* 可以自己先实现一遍，再看看别人或者书里是怎么实现的，之后可以做出相应调整，如果直接模仿抄一遍，不好理解整体项目逻辑，自己的收获也有限，面试时也很难答的好。

### 测试与升华
* ①先用本机电脑测试看看能不能连接上webserver,如果不能就打一些断点用GDB调试
* ②可以使用WebBench测试看看性能如何。
* ③可以试着边优化服务器性能边用WebBench测试看看效果。
* ④可以增加一些功能，如日志系统，cookie,管线化，或者支持更高的http版本，更多的方法，运用上mysql,redis等数据库，把webserver变成一个真正有功能的网站。