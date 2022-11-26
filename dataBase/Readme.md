<!--
 * @Author: RunningBeef 2723772192@qq.com
 * @Date: 2022-11-24 21:31:15
 * @LastEditors: RunningBeef 2723772192@qq.com
 * @LastEditTime: 2022-11-26 12:07:42
 * @FilePath: /lighthouse/WebServer/dataBase/Readme.md
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
-->
### 事先准备
* [前期配置与学习](https://blog.csdn.net/RunningBeef/article/details/127943352)

### 编译testMySQL.cc
* 1. 新建或修改MySQLConn.cc/.h 要重新生成对应动态连接库<br>```
g++ -shared -fPIC -o libMySQLConn.so MySQLConn.cc```<br>新建或修改ConnectPool.cc/.h要重新生产对应态链接库<br>```g++ -shared -fPIC -o libConnectionPool.so ConnectionPool.cc```

* 2. 编译连接生成testMySQLConn.exe  ``` g++  testMySQL.cc MySQLConn.cc ConnectionPool.cc  -lmysqlclient  -lpthread -lm -L../locker -lMutex -lCondition -L../json/lib -ljsoncpp -o testMySQL.exe```<br>```g++ -g -o  testMySQL testMySQL.cc MySQLConn.cc ConnectionPool.cc ../locker/Mutex.cc ../locker/Condition.cc -lmysqlclient -lpthread -lm -L../json/lib -ljsoncpp```
* 3. 启动service mysql start启动mysql服务
* 4. 指定LD_LIBRARY_PATH=. ./testMySQL.exe
* 关于动态库静态库使用介绍。
> https://www.zhihu.com/question/20484931<br>
https://www.runoob.com/w3cnote/cpp-static-library-and-dynamic-library.html<br>https://cloud.tencent.com/developer/article/1711778<br>
* 过程中可能出现的"undefined reference to"
> https://blog.csdn.net/aiwoziji13/article/details/7330333
### 编译testJson.cc
* g++ testJosn.cc -L../json/lib -ljsoncpp -o testJson.exe
* ./testJson.exe