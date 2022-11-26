/*
 * @Author: RunningBeef 2723772192@qq.com
 * @Date: 2022-11-21 22:56:44
 * @LastEditors: RunningBeef 2723772192@qq.com
 * @LastEditTime: 2022-11-22 12:16:00
 * @FilePath: /lighthouse/WebServer/dataBase/test.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>
#include <fstream>
#include "../json/include/json/json.h"
using namespace Json;
void write()
{
      Value root;
      root.append("lucy");
      root.append(19);
      root.append(false);

      Value sub;
      sub.append("apple");
      sub.append("false");
      root.append(sub);

      Value obj;
      obj["sex"] = "man";
      obj["girlfriend"] = "H";
      root.append(obj);

      std::string aJson = root.toStyledString();
      FastWriter w;
      std::string bJson = w.write(root);

      std::ofstream ofs("test.json");
      ofs << bJson;
      ofs.close();
}

void read()
{
      std::ifstream ifs("test.json");
      Reader rd;
      Value root;
      rd.parse(ifs, root);

      if (root.isArray())
      {
            for (int i = 0; i < root.size(); ++i)
            {
                  Value item = root[i];
                  if (item.isInt())
                  {
                        std::cout << item.asInt() << std::endl;
                  }
                  else if (item.isString())
                  {
                        std::cout << item.asString() << std::endl;
                  }
                  else if (item.isBool())
                  {
                        std::cout << item.asBool() << std::endl;
                  }
                  else if (item.isArray())
                  {
                        for (int i = 0; i < item.size(); ++i)
                        {
                              std::cout << item.isString() << std::endl;
                        }
                  }
                  else if (item.isObject())
                  {
                        Value::Members keys = item.getMemberNames();
                        for (int i = 0; i < keys.size(); ++i)
                        {
                              std::cout << keys.at(i) << " : " << item[keys[i]] << std::endl;
                        }
                        std::cout << std::endl;
                  }
            }
      }
      else if (root.isObject())
      {
      }
}
int main()
{
      write();
      read();
      return 0;
}