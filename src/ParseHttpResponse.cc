#include "../include/ParseHttpResponse.h"

ParseHttpResponse::
    ParseHttpResponse(std::shared_ptr<HttpRequest> http_request_ptr,
                      std::shared_ptr<HttpResponse> http_response_ptr)
    : http_request_ptr_(http_request_ptr), http_response_ptr_(http_response_ptr)
{
}

void ParseHttpResponse::
    parse()
{
    
}