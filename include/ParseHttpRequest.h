#ifndef PARSEHTTPREQUEST_H
#define PARSEHTTPREQUEST_H
class ParseHttpRequest
{
public:
      enum class HttpParseState{
            KRequestLine = 0,
            KRequestHeader,
            KRequestBody,
            KRequestOk
      };
      enum class LineStatus{
            KLineMore = 0,
            KLineOk,
            KLineBad
      };

private:
};
#endif