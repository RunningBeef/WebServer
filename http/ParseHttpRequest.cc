#include "ParseHttpRequest.h"

ParseHttpRequest::
    ParseHttpRequest(std::shared_ptr<HttpRequest> &http_request_ptr, std::shared_ptr<ClientSocket> & client_socket_ptr)
    : m_http_request_ptr_(http_request_ptr),
      m_client_socket_ptr_(client_socket_ptr)
{
      init();
      m_buffer_ = new char[BUFFERSIZE];
}
ParseHttpRequest::~ParseHttpRequest()
{
      if(m_buffer_)
      {
            delete m_buffer_;
      }
}
void ParseHttpRequest::init()
{
      m_checked_ = m_uncheck_ = m_end_ = m_body_length_ = 0;
      bzero(m_buffer_, BUFFERSIZE);
}
ParseHttpRequest::LineStatus ParseHttpRequest::
    parseOneLine()
{
      for (; m_uncheck_ < m_end_; ++m_uncheck_)
      {
            if (m_buffer_[m_uncheck_] == LF)
            {
                  if (!m_uncheck_ || m_buffer_[m_uncheck_ - 1] != CR)
                        return LineStatus::KLineBad;
                  ++m_uncheck_;
                  m_buffer_[m_uncheck_ - 1] = m_buffer_[m_uncheck_ - 2] = END;
                  return LineStatus::KLineOk;
            }
            if (m_buffer_[m_uncheck_] == CR)
            {
                  if (m_uncheck_ + 1 == m_end_)
                        return LineStatus::KLineMore;
                  if (m_buffer_[m_uncheck_ + 1] != LF)
                        return LineStatus::KLineBad;
                  m_uncheck_ += 2;
                  m_buffer_[m_uncheck_ - 1] = m_buffer_[m_uncheck_ - 2] = END;
                  return LineStatus::KLineOk;
            }
      }
      return LineStatus::KLineMore;
}

void ParseHttpRequest::
    parseRequestLine()
{
      HttpParseState &parseState = m_http_request_ptr_->m_parseResult;
      std::string requestLine(m_buffer_ + m_checked_ + 1, m_buffer_ + m_uncheck_);
      // 不用buffer + unchecked - 2 后面'\0' 会在is >> httpVersion是过滤掉
      std::istringstream is(requestLine);
      std::string method, url, httpVersion;
      is >> method >> url >> httpVersion;
#ifdef DEBUG
      cout << "method: " << method << "/t"
           << "url: " << url << "\t"
           << "httpVersion: " << httpVersion << std::endl;
#endif
      auto it = HttpRequest::KHttpMethodMap.find(method);
      if (it == HttpRequest::KHttpMethodMap.end())
      {
#ifdef DEBUG
            cout << "!ERROR Method: " << method << "not support" << std::endl;
#endif
            parseState = HttpParseState::KBadRequest;
      }
      m_http_request_ptr_->setMethod(it->second);
      m_http_request_ptr_->setUrl(url);

      if (httpVersion == "HTTP/1.1")
            m_http_request_ptr_->setHttpVersion(HttpRequest::HttpVersion::KHttp1_1);
      else if (httpVersion == "HTTP/1.0")
            m_http_request_ptr_->setHttpVersion(HttpRequest::HttpVersion::KHttp1_0);
      else
      {
#ifdef DEBUG
            cout << "!ERROR HttpVersion: " << httpVersion << "not support" << std::endl;
#endif
            m_http_request_ptr_->setHttpVersion(HttpRequest::HttpVersion::KVersionNotSupport);
            parseState = HttpParseState::KBadRequest;
      }
      parseState = HttpParseState::KParseHeader;
}

// 每个头部字段都以CRLF结尾
// 报文头结束的时候有额外的一个空行CRLF
// 我们每次都从socket缓冲区读到的数据中解析出一个以CRLF结尾的一行字符串，解析的时候把CRLF替换为'\0'方便处理
// 所以当读取到一个CRLF('\0''\0')我们可以断定头部部分已经结束了
void ParseHttpRequest::
    parseRequestHeader()
{
      HttpParseState &parse_state = m_http_request_ptr_->m_parseResult;
      if (m_buffer_[m_checked_ + 1] == '\0' && m_buffer_[m_checked_ + 1] == m_buffer_[m_checked_ + 2])
      {
            parse_state = HttpParseState::KParseBody;
            return;
      }
      std::string header(m_buffer_ + m_checked_ + 1, m_buffer_ + m_uncheck_);
      std::istringstream is(header);
      std::string key, value;
      is >> key >> value;
      key.pop_back();
      auto it = HttpRequest::KRequestHeaderMap.find(key);
      if (it == HttpRequest::KRequestHeaderMap.end())
      {
#ifdef DEBUG
            cout << "!ERROR head: " << key << "not found" << std::endl;
#endif;
      }
      else
      {
            m_http_request_ptr_->m_http_header_[it->second] = value;
            if (it->second == HttpRequest::HttpRequestHeader::KContent_Length)
            {
                  m_body_length_ = atol(value.c_str());
            }
      }
}

void ParseHttpRequest::
    parseRequestBody()
{
      HttpParseState &parse_state = m_http_request_ptr_->m_parseResult;
      if (m_http_request_ptr_->getHttpMethod() == HttpRequest::HttpMethod::KGet) // GET请求一般没有请求体
      {
            parse_state = HttpParseState::KGetRequest;
            return;
      }
      if (m_checked_ + m_body_length_ > m_end_)
      {
            return;
      }
      std::string body(m_buffer_ + m_checked_ + 1, m_buffer_ + m_end_);
      m_http_request_ptr_->m_body_ = body;
}

void ParseHttpRequest::
    parseRequest()
{
      HttpParseState &parse_state = m_http_request_ptr_->m_parseResult;
      LineStatus line_state = LineStatus::KLineOk;
      while ((line_state = parseOneLine()) == LineStatus::KLineOk)
      {
            switch (parse_state)
            {
            case HttpParseState::KParseLine:
                  parseRequestLine();
                  break;
            case HttpParseState::KParseHeader:
                  parseRequestHeader();
                  break;
            case HttpParseState::KParseBody:
                  parseRequestBody();
                  break;
            default:
                  return ;
            }
      }
}

//使用ET
bool ParseHttpRequest::parse()
{
      init();
      while (true)
      {
            int bytes_read = recv(m_client_socket_ptr_->getClientSocket(), m_buffer_ + m_end_, BUFFERSIZE - m_end_, 0);
            if (bytes_read == -1)
            {
                  if (errno == EAGAIN || errno == EWOULDBLOCK)
                        break;
                  return false;
            }
            else if (bytes_read == 0)
            {
                  return false;
            }
            m_end_ += bytes_read;
            parseRequest();
            switch (m_http_request_ptr_->m_parseResult)
            {
            case HttpParseState::KGetRequest: return true;
            case HttpParseState::KInternalError: return false;
            case HttpParseState::KBadRequest: return false;
            }
      }
      return true;
}