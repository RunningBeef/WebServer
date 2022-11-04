#include "../include/ParseHttpRequest.h"

ParseHttpRequest::
    ParseHttpRequest(HttpRequest &http_request, int &end, char *buffer)
    : http_request_(http_request), end_(end), buffer_(buffer)
{
      uncheck_ = checked_ = 0;
}
LineStatus ParseHttpRequest::
    parseOneLine()
{
      for (; uncheck_ < end_; ++uncheck_)
      {
            if (buffer_[uncheck_] == LF)
            {
                  if (!uncheck_ || buffer_[uncheck_ - 1] != CR)
                        return LineStatus::KLineBad;
                  ++uncheck_;
                  buffer_[uncheck_ - 1] = buffer_[uncheck_ - 2] = END;
                  return LineStatus::KLineOk;
            }
            if (buffer_[uncheck_] == CR)
            {
                  if (uncheck_ + 1 == end_)
                        return LineStatus::KLineMore;
                  if (buffer_[uncheck_ + 1] != LF)
                        return LineStatus::KLineBad;
                  uncheck_ += 2;
                  buffer_[uncheck_ - 1] = buffer_[uncheck_ - 2] = END;
                  return LineStatus::KLineOk;
            }
      }
      return LineStatus::KLineMore;
}

void ParseHttpRequest::
    parseRequestLine(HttpParseState &parseState)
{
      std::string requestLine(buffer_ + checked_ + 1, buffer_ + uncheck_);
      // 不用buffer + unchecked - 2 后面'\0' 会在is >> httpVersion是过滤掉
      std::istringstream is(requestLine);
      std::string method, url, httpVersion;
      is >> method >> url >> httpVersion;
#ifdef DEBUG
      cout << "method: " << method << "/t"
           << "url: " << url << "\t"
           << "httpVersion: " << httpVersion << std::endl;
#endif
      auto it = HttpRequest::string_to_http_method.find(method);
      if (it == HttpRequest::string_to_http_method.end())
      {
#ifdef DEBUG
            cout << "!ERROR Method: " << method << "not support" << std::endl;
#endif
            parseState = HttpParseState::KRequestBad;
      }
      http_request_.setMethod(it->second);
      http_request_.setUrl(url);

      if (httpVersion == "HTTP/1.1")
            http_request_.setHttpVersion(HttpRequest::HttpVersion::KHttp1_1);
      else if (httpVersion == "HTTP/1.0")
            http_request_.setHttpVersion(HttpRequest::HttpVersion::KHttp1_0);
      else
      {
#ifdef DEBUG
            cout << "!ERROR HttpVersion: " << httpVersion << "not support" << std::endl;
#endif
            http_request_.setHttpVersion(HttpRequest::HttpVersion::KVersionNotSupport);
            parseState = HttpParseState::KRequestBad;
      }
      parseState = HttpParseState::KParseHeader;
}

// 每个头部字段都以CRLF结尾
// 报文头结束的时候有额外的一个空行CRLF
// 我们每次都从socket缓冲区读到的数据中解析出一个以CRLF结尾的一行字符串，解析的时候把CRLF替换为'\0'方便处理
// 所以当读取到一个CRLF('\0''\0')我们可以断定头部部分已经结束了
void ParseHttpRequest::
    parseRequestHeader(HttpParseState &parse_state)
{
      if (buffer_[checked_ + 1] == '\0' && buffer_[checked_ + 1] == buffer_[checked_ + 2])
      {
            if (http_request_.getHttpMethod() == HttpRequest::HttpMethod::KGet) // GET请求一般没有请求体
            {
                  parse_state = HttpParseState::KRequestOk;
            }
            parse_state = HttpParseState::KParseBody;
            return;
      }
      std::string header(buffer_ + checked_ + 1, buffer_ + uncheck_);
      std::istringstream is(header);
      std::string key, value;
      is >> key >> value;
      key.pop_back();
      auto it = HttpRequest::string_to_http_header.find(key);
      if (it == HttpRequest::string_to_http_header.end())
      {
#ifdef DEBUG
            cout << "!ERROR head: " << key << "not found" << std::endl;
#endif;
      }
      else
      {
            http_request_.http_header_[it->second] = std::pair<std::string, std::string>(key, value);
      }
}

void ParseHttpRequest::
    parseRequestBody(HttpParseState &httpParseState)
{
      std::string body(buffer_ + checked_ + 1, buffer_ + uncheck_);
      http_request_.setHttpBody(body);
}

void ParseHttpRequest::
    parseRequest(HttpParseState &parse_state)
{
      LineStatus line_state = LineStatus::KLineOk;
      while ((line_state = parseOneLine()) == LineStatus::KLineOk)
      {
            switch (parse_state)
            {
            case HttpParseState::KParseLine:
                  parseRequestLine(parse_state);
                  break;
            case HttpParseState::KParseHeader:
                  parseRequestHeader(parse_state);
                  break;
            case HttpParseState::KParseBody:
                  parseRequestBody(parse_state);
                  break;
            default:
                  return;
            }
      }
}