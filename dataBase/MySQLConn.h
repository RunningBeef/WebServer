/*
 * @Author: RunningBeef 2723772192@qq.com
 * @Date: 2022-11-15 11:40:20
 * @LastEditors: RunningBeef 2723772192@qq.com
 * @LastEditTime: 2022-11-25 11:30:36
 * @FilePath: /lighthouse/WebServer/dataBase/MySQLConn.h
 * @Description: this is a mysql connection
 */
#ifndef MYSQLCONN_H
#define MYSQLCONN_H
#include <string>
#include <iostream>
#include <mysql/mysql.h>
#include <sys/time.h>

class MySQLConn{
public:
      //初始化数据库连接 构造
      MySQLConn();
      //是否数据库连接 析构
      ~MySQLConn();
      //连接数据库
      bool connect(std::string user, std::string password, std::string db_name,
      std::string ip, unsigned short port = 3306);
      //更新数据库：insert,update,delete
      bool update(std::string & sql);
      //查询数据库
      bool query(std::string & sql);
      //遍历查询到的结果集
      bool next();
      //得到结果集中的字段值
      std::string value(int index);
      //事物操作
      bool transaction();
      //提交事物
      bool commit();
      //事物回滚
      bool rollback();
      //刷新起始空闲时间点
      void refreshAliveTime();
      //返回空闲时长
      size_t getIdleTime();
private:
      //释放结果集内存地址 注意内部维护，否则有内存泄漏
      void freeResult();
      MYSQL * m_conn = nullptr;
      MYSQL_RES * m_result = nullptr;
      MYSQL_ROW m_row = nullptr;//本身是个二级指针
      size_t m_alive_time;
};

#endif