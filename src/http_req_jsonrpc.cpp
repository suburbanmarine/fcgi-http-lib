/**
 * This file is part of fcgi-http-lib, a library with utilities for handling basic HTTP requests over FCGI.
 * 
 * This software is distrubuted in the hope it will be useful, but without any warranty, including the implied warrranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See LICENSE.txt for details.
 * 
 * @author Jacob Schloss <jacob.schloss@suburbanmarine.io>
 * @copyright Copyright (c) 2021 Suburban Marine, Inc. All rights reserved.
 * @license Licensed under the LGPL-3.0 license. See LICENSE.txt for details.
 * SPDX-License-Identifier: LGPL-3.0-only
*/

#include "http-bridge/http_req_jsonrpc.hpp"
#include "http-bridge/http_req_util.hpp"
#include "http-bridge/http_util.hpp"
#include "http-bridge/http_common.hpp"
#include "http-bridge/content_type.hpp"

#include <jsonrpccxx/server.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>

void http_req_jsonrpc::handle(FCGX_Request* const request)
{
  http_req_util req_util;
  req_util.load(request);
  req_util.log_request_env();

  //GET, POST, ...
  // req_util.REQUEST_METHOD
  //DOCUMENT_URI is just path
  // req_util.DOCUMENT_URI
  //REQUEST_URI is path and query string
  // req_util.REQUEST_URI
  //verify this is application/json; charset=UTF-8
  // req_util.CONTENT_TYPE
  //verify content size is sane
  // req_util.CONTENT_LENGTH

  switch(req_util.request_method_enum)
  {
    case http_common::REQUEST_METHOD::OPTIONS:
    {
      handle_req_option(request, req_util);
      break;
    }
    case http_common::REQUEST_METHOD::POST:
    {
      handle_req_post(request, req_util);
      break;      
    }
    default:
    {
      throw MethodNotAllowed("Only POST and OPTIONS are accepted");
      break;
    }
  }

  FCGX_Finish_r(request);
}

void http_req_jsonrpc::handle_req_option(FCGX_Request* const request, const http_req_util& req_util)
{
  // TODO tighten scope of Access-Control-Allow-Origin
  FCGX_PutS("Status: 200 OK\r\n"                            , request->out);
  FCGX_PutS("Access-Control-Allow-Origin: *\r\n"            , request->out);
  FCGX_PutS("Access-Control-Allow-Methods: POST\r\n"        , request->out);
  FCGX_PutS("Access-Control-Allow-Headers: Content-Type\r\n", request->out);
  FCGX_PutS("Access-Control-Max-Age: 86400\r\n"             , request->out);
  FCGX_PutS("\r\n", request->out);
}
void http_req_jsonrpc::handle_req_post(FCGX_Request* const request, const http_req_util& req_util)
{
  int req_len = 0;
  int ret = sscanf(req_util.CONTENT_LENGTH, "%d", &req_len);
  if(ret != 1)
  {
    throw BadRequest("Could not parse CONTENT_LENGTH");
  }

  if((req_len < 0) || (req_len > MAX_REQ_LEN))
  {
    throw PayloadTooLarge();
  }

  {
    http_common::Content_type content_type;
    if( ! http_common::parse_content_type(req_util.CONTENT_TYPE, &content_type) )
    {
      throw UnsupportedMediaType("Could not parse CONTENT_TYPE");   
    } 

    if(content_type.media_type != APP_JSON)
    {
      throw UnsupportedMediaType();
    }
  }

  //hold the jsonrpc response object
  std::string result_str;

  //this part of the rpc is single threaded
  {
    std::lock_guard<std::mutex> lock(m_jsonrpc_mutex);

    //read to the vector, then the string
    {
      m_req_buf.resize(req_len);
      const int bytes_in = FCGX_GetStr(m_req_buf.data(), m_req_buf.size(), request->in);
      if(bytes_in != req_len)
      {
        throw InternalServerError("Read did not return expected length");
      }

      //copy to a string to make jsonrpc-lean happy
      //maybe patch jsonrpc-lean to take a string-view
      m_req_str.assign(m_req_buf.data(), bytes_in);
      SPDLOG_DEBUG("Request: {:.{}}",  m_req_buf.data(), bytes_in);
    }

    try
    {
      result_str = m_jsonrpc_server_ptr->HandleRequest(m_req_str);
    }
    catch(std::exception& e)
    {
      SPDLOG_ERROR("Error handling jsonrpc request: {:s}", e.what());
      throw InternalServerError("Error handling jsonrpc request");
    }
  }

  //trace logging
  if(!result_str.empty())
  {
    SPDLOG_DEBUG("Response: {:.{}}", result_str.data(), result_str.size());
  }
  else
  {
    SPDLOG_ERROR("Response: Empty");
  }

  if(false)
  {
    // TODO tighten scope of Access-Control-Allow-Origin
    FCGX_PutS("Access-Control-Allow-Origin: *\r\n"            , request->out);
    FCGX_PutS("Access-Control-Allow-Methods: POST\r\n"        , request->out);
    FCGX_PutS("Access-Control-Allow-Headers: Content-Type\r\n", request->out);
    FCGX_PutS("Access-Control-Max-Age: 86400\r\n"             , request->out);
  }

  // -32700  Parse error Invalid JSON was received by the server.
  // An error occurred on the server while parsing the JSON text.
  // -32600  Invalid Request The JSON sent is not a valid Request object.
  // -32601  Method not found  The method does not exist / is not available.
  // -32602  Invalid params  Invalid method parameter(s).
  // -32603  Internal error  Internal JSON-RPC error.
  // -32000 to -32099  Server error  Reserved for implementation-defined server-errors.
  // FCGX_PutS("{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32000, \"message\": \"Server Error\"}, \"id\": 1}", request->out);
  if(result_str.size() == 0)
  {
    //notification, no jsonrpc resp
    FCGX_PutS("Content-Type: application/json\r\n"    , request->out);
    FCGX_PutS("Content-Length: 0\r\n"                 , request->out);
    FCGX_PutS("Status: 204 No Response\r\n"           , request->out);
    FCGX_PutS("Cache-Control: max-age=0, no-store\r\n", request->out);
    FCGX_PutS("\r\n"                                  , request->out);
  }
  else
  {
    //print response header
    FCGX_PutS("Content-Type: application/json\r\n", request->out);
    FCGX_FPrintF(request->out, "Content-Length: %d\r\n", result_str.size());
    FCGX_PutS("Status: 200 OK\r\n", request->out);
    FCGX_PutS("Cache-Control: max-age=0, no-store\r\n", request->out);
    FCGX_PutS("\r\n", request->out);

    //print response body
    FCGX_PutStr(result_str.data(), result_str.size(), request->out);
  } 
}