#include "MySQLConn.h"

MySQLConn::MySQLConn()
{
      m_conn = mysql_init(nullptr);
      //设置UTF8编码
      mysql_set_character_set(m_conn, "utf8");
}

MySQLConn::~MySQLConn()
{
      if (m_conn != nullptr)
      {
            mysql_close(m_conn);
      }
      freeResult();
}

bool MySQLConn::connect(std::string user, std::string password, std::string db_name,
std::string ip, unsigned short port = 3306)
{
      MYSQL * ptr = mysql_real_connect(m_conn, ip.c_str(), user.c_str(), password.c_str(), db_name.c_str(),port,nullptr,0);
      return ptr != nullptr;
}


bool MySQLConn::update(std::string & sql)
{
      return mysql_query(m_conn,sql.c_str()) == 0;

}

bool MySQLConn::query(std::string & sql)
{
      freeResult();
      if(mysql_query(m_conn,sql.c_str()))
      {
            return false;
      }
      //将服务端查询到的结果集保存到客户端
      //需要在客户端创建一块内存，所以函数返回客户端保存结果集的内存地址
      m_result = mysql_store_result(m_conn);
}

//移动结果集指针指向下一条记录
//注意这里如果有所以记录都遍历完，m-_row = nullptr
//但是内部没做保护，所以外部遍历sql结果要注意该函数调用次数
bool MySQLConn::next()
{
      if(m_result != nullptr)
      {
            //返回一个二级指针（指针数组，保存当前记录所有字段的值）
            m_row = mysql_fetch_row(m_result);
      }
      return false;
}

std::string MySQLConn::value(int index)
{
      int column_num = mysql_num_fields(m_result);
      if(index >= column_num || index < 0)
      {
            return "";
      }
      char * value = m_row[index];
      //数据可能有'\0'导致提前结束，所以需要获取实际字段长度
      int length = mysql_fetch_lengths(m_result)[index];
      return std::string(value,length);
}

bool MySQLConn::transaction()
{
      //设置事务提交为手动
      return mysql_autocommit(m_conn,false);
}

bool MySQLConn::commit()
{
      return mysql_commit(m_conn);
}

bool MySQLConn::rollback()
{
      return mysql_rollback(m_conn);
}

void MySQLConn::freeResult()
{
      if(m_result)
      {
            mysql_free_result(m_result);
            m_result = nullptr;
      }
}