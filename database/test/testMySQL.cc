/*
 * @Author: RunningBeef 2723772192@qq.com
 * @Date: 2022-11-24 10:59:03
 * @LastEditors: RunningBeef 2723772192@qq.com
 * @LastEditTime: 2022-11-26 22:40:10
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

int main()
{
      testSingleThread(20000);
      // 5000 time write
      // no pool cost: 17407016387 纳秒，17407毫秒
      // pool cost: 13153238192 纳秒，13153毫秒
      //20000 time write
      //no pool cost: 66090708016 纳秒，66090毫秒
      //pool cost: 56846949259 纳秒，56846毫秒
      return 0;
}