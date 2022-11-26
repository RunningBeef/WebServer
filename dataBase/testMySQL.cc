/*
 * @Author: RunningBeef 2723772192@qq.com
 * @Date: 2022-11-24 10:59:03
 * @LastEditors: RunningBeef 2723772192@qq.com
 * @LastEditTime: 2022-11-26 17:06:56
 * @FilePath: /lighthouse/WebServer/dataBase/testMySQL.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>
#include <memory>
#include "MySQLConn.h"
#include "ConnectionPool.h"

void testSingleThread(int num)
{
      size_t beg, end;
      struct timeval cur;
#if 0

      gettimeofday(&cur, NULL);
      beg = cur.tv_sec;
      for (int i = 0; i < num; ++i)
      {
            MySQLConn conn;
            bool ret = conn.connect("root", "Apple38512", "webServer", "localhost", 3306);
            std::string sql = "INSERT INTO user values('hello', 'world');";
            ret = conn.update(sql);
      }
      gettimeofday(&cur, NULL);
      end = cur.tv_sec;
      std::cout << "no pool cost: " << end - beg << std::endl;
#else
      gettimeofday(&cur, NULL);
      beg = cur.tv_sec;
      ConnectionPool *conn_pool_ptr = ConnectionPool::getConnectionPool();
      for (int i = 0; i < num; ++i)
      {
            std::shared_ptr<MySQLConn> conn = conn_pool_ptr->getConnection();
            std::string sql = "INSERT INTO user values('hello', 'world');";
            bool ret = conn->update(sql);
      }
      gettimeofday(&cur, NULL);
      end = cur.tv_sec;
      std::cout << "pool cost: " << end - beg << std::endl;
#endif
}

int main()
{
      testSingleThread(5000);
      // no pool cost 16sec
      //
      return 0;
}