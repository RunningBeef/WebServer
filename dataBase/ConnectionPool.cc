/*
 * @Author: RunningBeef 2723772192@qq.com
 * @Date: 2022-11-20 18:22:03
 * @LastEditors: RunningBeef 2723772192@qq.com
 * @LastEditTime: 2022-11-26 16:59:38
 * @FilePath: /lighthouse/WebServer/dataBase/ConnectionPool.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "ConnectionPool.h"
ConnectionPool::ConnectionPool() : m_produce_condition(m_mutex), m_consume_condition(m_mutex), m_shutdown(false)
{
      if (!parseJsonFile())
      {
            return;
      }
      for (int i = 0; i < m_min_size; ++i)
      {
            addConnection();
      }
      if (pthread_create(&m_produce_id, NULL, produceConnection, this) || pthread_detach(m_produce_id))
      {
            m_shutdown = true;
            return;
      }
      if (pthread_create(&m_recycle_id, NULL, recycleConnection, this) || pthread_detach(m_recycle_id))
      {
            m_shutdown = true;
            return;
      }
}
ConnectionPool::~ConnectionPool()
{
      m_shutdown = true;
      MutexGard mutex_gard(m_mutex);
      while (!m_connectionQ.empty())
      {
            MySQLConn *conn = m_connectionQ.front();
            m_connectionQ.pop();
            delete conn;
      }
      /* 一定要注意用条件变量唤醒其他线程，
      让其检测到m_shutdown然后退出，
      否则这些线程继续共享条件变量，互斥锁，那么连接池就会卡着无法析构*/
      m_produce_condition.broadCast();
      m_consume_condition.broadCast();
#ifdef DEBUG
      std::cout << "ConnectionPool destroy success" << std::endl;
#endif
}
ConnectionPool *
ConnectionPool::getConnectionPool()
{
      //静态局部变量是线程安全的
      static ConnectionPool pool;
      return &pool;
}

bool ConnectionPool::parseJsonFile()
{
      std::ifstream json_file("MySQLConf.json");
      Json::Reader rd;
      Json::Value root;
      rd.parse(json_file, root);
      if (root.isObject())
      {
            m_ip = root["ip"].asString();
            m_port = root["port"].asUInt();
            m_username = root["username"].asString();
            m_password = root["password"].asString();
            m_db_name = root["dbName"].asString();
            m_min_size = root["minSize"].asInt64();
            m_max_size = root["maxSize"].asInt64();
            m_max_idle_time = root["maxIdleTime"].asInt64();
            m_timeout = root["timeout"].asInt64();
            return true;
      }
      if (m_min_size <= 0 || m_max_size < m_min_size)
      {
            return false;
      }
      return false;
}

//维护连接池中可用连接的个数，如果被唤醒并且连接不足就生产
void *ConnectionPool::produceConnection(void *arg)
{
      ConnectionPool *conn_pool_ptr = (ConnectionPool *)arg;
      while (true)
      {
            MutexGard mutex_gard(conn_pool_ptr->m_mutex);
            while (conn_pool_ptr->m_connectionQ.size() >= conn_pool_ptr->m_min_size && !conn_pool_ptr->m_shutdown)
            {
                  conn_pool_ptr->m_produce_condition.wait();
            }
            if (conn_pool_ptr->m_shutdown)
                  return nullptr;
            int produce_num = conn_pool_ptr->m_connectionQ.size() > 4 ? 1 : 3;
            while (produce_num--)
            {
                  conn_pool_ptr->addConnection();
            }
            conn_pool_ptr->m_consume_condition.broadCast();
      }
}

//维护连接池中可用连接个数，如果连接数过多并且连接过度空闲就释放
void *ConnectionPool::recycleConnection(void *arg)
{
      ConnectionPool *conn_pool_ptr = (ConnectionPool *)arg;
      while (true)
      {
            sleep(1);
            MutexGard mutex_gard(conn_pool_ptr->m_mutex);
            int max_release_num = 500;
            while (conn_pool_ptr->m_connectionQ.size() > conn_pool_ptr->m_max_size && !conn_pool_ptr->m_shutdown && max_release_num--)
            {

                  MySQLConn *conn = conn_pool_ptr->m_connectionQ.front();
                  if (conn->getIdleTime() > conn_pool_ptr->m_max_idle_time)
                  {
                        conn_pool_ptr->m_connectionQ.pop();
                        delete conn;
                  }
            }
            if (conn_pool_ptr->m_shutdown)
            {
                  return nullptr;
            }
      }
}

void ConnectionPool::addConnection()
{
      MySQLConn *conn = new MySQLConn;
      if (!conn->connect(m_username, m_password, m_db_name, m_ip, m_port))
      {
            return;
      }
      conn->refreshAliveTime();
      m_connectionQ.push(conn);
}

//为什么是用broadcast要注意
//那些地方会涉及连接队列？
//围绕需求：发现连接不够，能够让生产者主动生产
std::shared_ptr<MySQLConn> ConnectionPool::getConnection()
{
      MutexGard mutex_gard(m_mutex);
      while (m_connectionQ.empty())
      {
            m_produce_condition.signal();
            m_consume_condition.wait();
      }
      //通过指定析构函数，来自动回收连接
      std::shared_ptr<MySQLConn> conn_ptr(m_connectionQ.front(), [this](MySQLConn *conn)
                                          {
            //注意线程安全
            MutexGard mutex_gard(m_mutex);
            conn->refreshAliveTime();
            m_connectionQ.push(conn); });
      m_connectionQ.pop();
      return conn_ptr;
}