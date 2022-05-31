//
// Created by yonggeng on 3/24/22.
//

#include <algorithm>
#include "../include/ParseHttpRequest.h"
#include "../include/Util.h"

std::unordered_map<std::string, HttpRequest::HTTP_HEADER> HttpRequest::static_stringToHeader_map_ = {
        {"HOST",                      HttpRequest::Host},
        {"USER-AGENT",                HttpRequest::User_Agent},
        {"CONNECTION",                HttpRequest::Connection},
        {"ACCEPT-ENCODING",           HttpRequest::Accept_Encoding},
        {"ACCEPT-LANGUAGE",           HttpRequest::Accept_Language},
        {"ACCEPT",                    HttpRequest::Accept},
        {"CACHE-CONTROL",             HttpRequest::Cache_Control},
        {"UPGRADE-INSECURE-REQUESTS", HttpRequest::Upgrade_Insecure_Requests}
};

/*从读到的数据中取出一行*/
ParseHttpRequest::LINE_STATUS ParseHttpRequest::get_line(char * buffer,int & checked_index,int & read_index){
    bool error = false;
    for(;checked_index < read_index; ++checked_index)
    {
        if(buffer[checked_index] == CR){
            if((checked_index + 1) == read_index){/*数据不够，继续从SOCKET中读取数据，下次继续从checked_index开始检查*/
                return LINE_MORE;
            }
            else if(buffer[checked_index + 1] == LF){/*遇到CRLF说明读到完整的行*/
                buffer[checked_index ++ ] = STRING_END;/*CR---> '\0'*/
                buffer[checked_index ++ ] = STRING_END;/*LF---> '\0'*/
                return LINE_OK;
            }/*遇到单独的CR说明HTTP报文格式有问题*/
            else return LINE_BAD;
        }
        else if(buffer[checked_index] == LF){
            /*判断前一个是不是CR 其实这种情况会属于前面那种，所以没必要判断*/
            if(checked_index && buffer[checked_index - 1] == CR){
                buffer[checked_index - 1] = STRING_END;/*CR---> '\0'*/
                buffer[checked_index ++ ] = STRING_END;/*LF---> '\0'*/
                return LINE_OK;
            }else return LINE_BAD;
        }
    }
    return LINE_MORE;
}

/*处理请求行*/
/*method 空格 URL 空格 协议版本CRLF*/
ParseHttpRequest::HTTP_CODE ParseHttpRequest::parse_requestLine(char * temp,PARSE_STATUS & parseStatus,HttpRequest & httpRequest){
    char *url=strpbrk(temp," \t");/*当被检验字符在字符串 str2 中也包含时，则停止检验，并返回该字符位置*/
    /*如果请求行中没有空白字符或"\t"字符，则HTTP请求有问题*/
    if(!url){
        return BAD_REQUEST;
    }
    *url++=STRING_END;
    /*get method. url located int first block*/


    char*method=temp;
    /*strcasecmp忽略大小比较*/
    if(strcasecmp(method,"GET")==0){
        httpRequest.method_ = HttpRequest::GET;
        std::cout<< "The request method is GET" << std::endl;
    }else if(strcasecmp(method,"HEAD")==0){
        httpRequest.method_ = HttpRequest::HEAD;
        std::cout<< "The request method is HEAD" << std::endl;
    }else if(strcasecmp(method,"POST")==0){
        httpRequest.method_ = HttpRequest::POST;
        std::cout<< "The request method is POST" << std::endl;
    }else if(strcasecmp(method,"PUT")==0){
        httpRequest.method_ = HttpRequest::PUT;
        std::cout<< "The request method is PUT" << std::endl;
    }else{
        return BAD_REQUEST;
    }


    /*检索字符串 str1 中第一个不在字符串 str2 中出现的字符下标。*/
    url += strspn(url," \t");
    char*version = strpbrk(url," \t");
    if(!version){
        return BAD_REQUEST;
    }

    /*get version. version located in second block*/
    *version++ = STRING_END;
    version += strspn(version," \t");
    if(strncasecmp(version,"HTTP/1.0",8) == 0){
        httpRequest.version_ = HttpRequest::HTTP1_0;
        std::cout << "The request version is HTTP/1.O" << std::endl;
    }else if(strncasecmp(version,"HTTP/1.1",8) == 0){
        httpRequest.version_ = HttpRequest::HTTP1_1;
        std::cout << "The request version is HTTP/1.1" << std::endl;
    }else{
        return BAD_REQUEST;
    }


    /*get url*/
    if(strncasecmp(url,"http://",7) == 0){
        url += 7;
        url = strchr(url,'/');
    }else if(strncasecmp(url,"/",1) == 0){

    }else{
        return BAD_REQUEST;
    }
    if(!url || *url != '/'){
        return BAD_REQUEST;
    }
    httpRequest.url_ = std::string(url);
    parseStatus = PARSE_STATUS::PARSE_HEADER;
    return NO_REQUEST;
}

/*处理请求头部字段*/
/*头部字段名:值CRLF*/
ParseHttpRequest::HTTP_CODE ParseHttpRequest::parse_header(char * line,PARSE_STATUS & parseStatus,HttpRequest & httpRequest){
    if (*line == '\0') {/*头部字段读取完毕*/
        if (httpRequest.method_ == HttpRequest::GET) {/*GET一般不包含body*/
            return GET_REQUEST;
        }
        parseStatus = PARSE_BODY;
        return NO_REQUEST;
    }

    // FIXME  char key[20]曾被缓冲区溢出, value[100]也被 chrome的user-agent 溢出
    char key[100], value[300];

    // FIXME 需要修改有些value里也包含了':'符号
    sscanf(line, "%[^:]:%[^:]", key, value);


    decltype(HttpRequest::static_stringToHeader_map_)::iterator it;
    std::string key_s(key);
    std::transform(key_s.begin(), key_s.end(), key_s.begin(), ::toupper);
    std::string value_s(value);
//    if (key_s == std::string("UPGRADE-INSECURE-REQUESTS")) {
//        return NO_REQUEST;
//    }

    if ((it = HttpRequest::static_stringToHeader_map_.find(trim(key_s))) != (HttpRequest::static_stringToHeader_map_.end())) {
        httpRequest.headerToStringMap_.insert(std::make_pair(it->second, trim(value_s)));
    } else {
        std::cout << "Header no support: " << key << " : " << value << std::endl;
    }

    return NO_REQUEST;
}

/*处理请求体*/
ParseHttpRequest::HTTP_CODE
ParseHttpRequest::parse_body(char *body, HttpRequest &request) {
    request.content_ = body;
    return GET_REQUEST;
}

ParseHttpRequest::HTTP_CODE ParseHttpRequest::pare_httpRequest(char * buffer,int & checked_index, int & read_index,int & next_line_index,PARSE_STATUS &parseStatus,HttpRequest & httpRequest){
        LINE_STATUS lineStatus=LINE_OK;
        HTTP_CODE retCode = NO_REQUEST;
        /*只要从SOCKET中读取到的数据够用，就不断解析，并且每部分解析成功后就转移到下一个状态*/
        while((lineStatus = get_line(buffer,checked_index,read_index)) == LINE_OK){
            char * temp = buffer + next_line_index;
            next_line_index = checked_index;
            switch (parseStatus) {
                case PARSE_REQUESTLINE:
                {
                    retCode = parse_requestLine(temp,parseStatus,httpRequest);
                    if(retCode == BAD_REQUEST){
                        return BAD_REQUEST;
                    }
                }
                case PARSE_HEADER:
                {
                    retCode = parse_header(temp,parseStatus,httpRequest);
                    if(retCode == BAD_REQUEST){
                        return BAD_REQUEST;
                    } else if(retCode == GET_REQUEST){/*没有请求体,请求报文处理结束，直接返回*/
                        return GET_REQUEST;
                    }
                }
                case PARSE_BODY:
                {
                    retCode = parse_body(temp,httpRequest);
                    if(retCode == GET_REQUEST){/*处理结束直接返回*/
                        return GET_REQUEST;
                    }

                    return BAD_REQUEST;
                }
                default:
                {
                    return INTERNAL_ERROR;
                }
            }
        }
        /*可能从socket中读取的数据不够*/
        if(lineStatus==LINE_MORE){
            return NO_REQUEST;
        }else{/*否则说明*/
            return BAD_REQUEST;
        }
}
