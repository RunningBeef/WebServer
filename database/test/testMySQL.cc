/*
 * @Author: RunningBeef 2723772192@qq.com
 * @Date: 2022-11-24 10:59:03
 * @LastEditors: RunningBeef 2723772192@qq.com
 * @LastEditTime: 2022-11-27 11:27:19
 * @FilePath: /lighthouse/WebServer/dataBase/testMySQL.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>
#include <memory>
#include <chrono>
#include "../MySQLConn.h"
#include "../ConnectionPool.h"

void testSingleThread(int num)
{
#ifdef OPTION
      std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

      for (int i = 0; i < num; ++i)
      {
            MySQLConn conn;
            bool ret = conn.connect("root", "Apple38512", "webServer", "localhost", 3306);
            std::string sql = "INSERT INTO user values('hello', 'world');";
            ret = conn.update(sql);
      }
      std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
      auto length = end - begin;
      std::cout << "no pool cost: " << length.count() << " 纳秒，"
                << length.count() / 1000000 << "毫秒" << std::endl;
#else
      std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

      ConnectionPool *conn_pool_ptr = ConnectionPool::getConnectionPool();
      for (int i = 0; i < num; ++i)
      {
            std::shared_ptr<MySQLConn> conn = conn_pool_ptr->getConnection();
            std::string sql = "INSERT INTO user values('hello', 'world');";
            bool ret = conn->update(sql);
      }
      std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
      auto length = end - begin;
      std::cout << "pool cost: " << length.count() << " 纳秒，"
                << length.count() / 1000000 << "毫秒" << std::endl;
#endif
}

void testWrite(int num)
{
#ifdef OPTION
      std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
      MySQLConn conn;
      bool ret = conn.connect("root", "Apple38512", "webServer", "localhost", 3306);
      for (int i = 0; i < num; ++i)
      {
            std::string sql = "INSERT INTO user values('hello', 'world');";
            ret = conn.update(sql);
      }
      std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
      auto length = end - begin;
      std::cout << "no pool only write cost: " << length.count() << " 纳秒，"
                << length.count() / 1000000 << "毫秒" << std::endl;
#else
      std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

      ConnectionPool *conn_pool_ptr = ConnectionPool::getConnectionPool();
      std::shared_ptr<MySQLConn> conn = conn_pool_ptr->getConnection();
      for (int i = 0; i < num; ++i)
      {

            std::string sql = "INSERT INTO user values('hello', 'world');";
            bool ret = conn->update(sql);
      }
      std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
      auto length = end - begin;
      std::cout << "pool only write cost: " << length.count() << " 纳秒，"
                << length.count() / 1000000 << "毫秒" << std::endl;
#endif
}
void testGetConnection(int num)
{
#ifdef OPTION
      std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

      for (int i = 0; i < num; ++i)
      {
            MySQLConn conn;
            bool ret = conn.connect("root", "Apple38512", "webServer", "localhost", 3306);
      }
      std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
      auto length = end - begin;
      std::cout << "no pool no write cost: " << length.count() << " 纳秒，"
                << length.count() / 1000000 << "毫秒" << std::endl;
#else
      std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

      ConnectionPool *conn_pool_ptr = ConnectionPool::getConnectionPool();
      for (int i = 0; i < num; ++i)
      {
            std::shared_ptr<MySQLConn> conn = conn_pool_ptr->getConnection();
      }
      std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
      auto length = end - begin;
      std::cout << "pool no write cost: " << length.count() << " 纳秒，"
                << length.count() / 1000000 << "毫秒" << std::endl;
#endif
}

void *op(void *arg)
{
      ConnectionPool *conn_pool_ptr = (ConnectionPool *)arg;
      for (int i = 0; i < 1000; ++i)
      {
            std::shared_ptr<MySQLConn> conn = conn_pool_ptr->getConnection();
            std::string sql = "INSERT INTO user values('hello', 'world');";
            bool ret = conn->update(sql);
      }
}
void testMultiThread()
{
      std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
      ConnectionPool *pool_ptr = ConnectionPool::getConnectionPool();
      pthread_t pId[5];
      for (int i = 0; i < 5; ++i)
      {
            pthread_create(&pId[i], NULL, op, pool_ptr);
      }
      for (int i = 0; i < 5; ++i)
      {
            pthread_join(pId[i], NULL);
      }
      std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
      auto length = end - begin;
      std::cout << "pool multithread cost: " << length.count() << " 纳秒，"
                << length.count() / 1000000 << "毫秒" << std::endl;
}
int main()
{
      testMultiThread();
      // testSingleThread(20000);
      // 5000 time write
      // no pool cost: 17407016387 纳秒，17407毫秒
      // pool cost: 13153238192 纳秒，13153毫秒
      // 20000 time write
      // no pool cost: 66090708016 纳秒，66090毫秒
      // pool cost: 56846949259 纳秒，56846毫秒

      // testWrite(20000);
      // 5000
      // pool only write cost: 14207930451 纳秒，14207毫秒
      // no pool only write cost: 13694417511 纳秒，13694毫秒
      // no pool only write cost: 14912327309 纳秒，14912毫秒
      // 20000
      // no pool only write cost: 57075806945 纳秒，57075毫秒
      // pool only write cost: 55456354442 纳秒，55456毫秒

      // testGetConnection(100000);
      // 5000
      // pool cost: 19248529 纳秒，19毫秒
      // no pool cost: 3151437961 纳秒，3151毫秒
      // 20000
      // pool no write cost: 14426044 纳秒，14毫秒
      // no pool no write cost: 1592225352 纳秒，1592毫秒
      // 100000
      // pool no write cost: 37203092 纳秒，37毫秒
      // no pool no write cost: 16582025828 纳秒，16582毫秒

      /* 对比testGetConnection()明显可以看到单纯从连接池取连接肯定是比每次初始化连接更快
      testSingleThread()每次取一个连接并且写也是更快，但是没那么明显，只快了几秒
      根据testWrite() 明显发现5000写操作，写的耗时占大头，
      所以其实数据库连接池对于对连接操作和多的行为，优化不明细*/
      return 0;
}