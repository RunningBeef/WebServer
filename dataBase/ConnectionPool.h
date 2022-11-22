/*
 * @Author: RunningBeef 2723772192@qq.com
 * @Date: 2022-11-20 18:22:32
 * @LastEditors: RunningBeef 2723772192@qq.com
 * @LastEditTime: 2022-11-22 20:06:05
 * @FilePath: /lighthouse/WebServer/dataBase/ConnectionPool.h
 * @Description: this is a singleton mysql connection pool
 */
#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOO_H
#include <queue>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include "MySQLConn.h"
#include "../locker/Mutex.h"
#include "../locker/Condition.h"
#include "../json/include/json/json.h"

class ConnectionPool
{
public:
      static ConnectionPool *getConnectionPool();
      ~ConnectionPool();
      ConnectionPool(const ConnectionPool &obj) = delete;

      ConnectionPool &operator=(const ConnectionPool &obj) = delete;
      ConnectionPool &operator=(ConnectionPool &&obj) noexcept = delete;

private:
      ConnectionPool();
      bool parseJsonFile();
      static void *produceConnection(void *);
      static void *recycleConnection(void *);
      void addConnection();

      pthread_t m_produce_id;
      pthread_t m_recycle_id;
      bool m_shutdown;
      std::string m_ip;
      std::string m_username;
      std::string m_password;
      std::string m_db_name;
      unsigned short m_port;
      int m_max_size; //连接数上限
      int m_min_size;
      int m_timeout;       //连接个数不够时的等待时长
      int m_max_idle_time; //连接空闲的时长，超过该时长连接被释放
      std::queue<MySQLConn *> m_connectionQ;
      Mutex m_mutex;         //保护m_connection_queue;
      Condition m_condition; //生产者消费者协同
};
#endif