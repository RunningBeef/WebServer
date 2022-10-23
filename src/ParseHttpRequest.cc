#include "../include/ParseHttpRequest.h"

ParseHttpRequest::LineStatus ParseHttpRequest::
parseOneLine(char * buffer, int &uncheck, int & end)
{
      for(int i = uncheck; i < end; ++i){
            if(buffer[i] == LF)
            {
                  if(!i || buffer[i - 1] != CR)
                        return LineStatus::KLineBad;
                  uncheck = i + 1;
                  buffer[i - 1] = buffer[i] = END;
                  return LineStatus::KLineOk;
            }
            if(buffer[i] == CR)
            {
                  if(i + 1 == end)
                        return LineStatus::KLineMore;
                  if(buffer[i + 1] != LF)
                        return LineStatus::KLineBad;
                  buffer[i] = buffer[i + 1] = END;
                  uncheck = i + 2;
                  return LineStatus::KLineOk;
            }
      }
      uncheck = end;
      return LineStatus::KLineMore;
}

ParseHttpRequest::HttpParseState ParseHttpRequest::
parseRequestLine(char * buffer,int & checked,int & uncheck, HttpRequest &HttpRequest)
{
      int id = checked + 1;
      
}