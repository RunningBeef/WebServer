#include "../include/ParseHttpResponse.h"

ParseHttpResponse::
    ParseHttpResponse(HttpRequest &http_request, HttpResponse &http_response)
    : http_request_(http_request), http_response_(http_response)
{
}

void ParseHttpResponse::
    parse()
{
    
}