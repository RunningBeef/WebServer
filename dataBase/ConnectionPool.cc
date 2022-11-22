/*
 * @Author: RunningBeef 2723772192@qq.com
 * @Date: 2022-11-20 18:22:03
 * @LastEditors: RunningBeef 2723772192@qq.com
 * @LastEditTime: 2022-11-22 21:20:54
 * @FilePath: /lighthouse/WebServer/dataBase/ConnectionPool.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "ConnectionPool.h"
ConnectionPool::ConnectionPool() : m_condition(m_mutex), m_shutdown(false)
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
      return false;
}

//维护连接池中可用连接的个数，如果不足就生产
void *ConnectionPool::produceConnection(void *arg)
{
      ConnectionPool *conn_pool_ptr = (ConnectionPool *)arg;
      while (true)
      {
            MutexGard mutex_gard(conn_pool_ptr->m_mutex);
            while (conn_pool_ptr->m_connectionQ.size() >= conn_pool_ptr->m_min_size
                  && !conn_pool_ptr->m_shutdown)
            {
                  if(conn_pool_ptr->m_condition.wait())
                  {
                        return ;
                  }
            }
            if(conn_pool_ptr->m_shutdown)
                  return ;
            conn_pool_ptr->addConnection();
      }
}
//维护连接池中可用连接个数，如果过多就释放
void *ConnectionPool::recycleConnection(void *arg)
{
      ConnectionPool * conn_pool_ptr = (ConnectionPool * ) arg;
      while(true)
      {
            sleep(1);
            MutexGard mutex_gard(conn_pool_ptr->m_mutex);
            if(conn_pool_ptr->m_connectionQ.size() > conn_pool_ptr->m_max_size)
            {
                  
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
      m_connectionQ.push(conn);
}