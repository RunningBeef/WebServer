#ifndef MYSQLCONN_H
#define MYSQLCONN_H
#include <string>
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
private:
};

#endif