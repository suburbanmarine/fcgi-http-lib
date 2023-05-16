/**
 * @author Jacob Schloss <jacob.schloss@suburbanmarine.io>
 * @copyright Copyright (c) 2021 Suburban Marine, Inc. All rights reserved.
 * @license Licensed under the 3-Clause BSD LICENSE. See LICENSE.txt for details.
*/

#include "http-bridge/http_req_jsonrpc.hpp"
#include "http-bridge/http_req_util.hpp"
#include "http-bridge/http_util.hpp"
#include "http-bridge/http_common.hpp"

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

  {
    if(req_util.request_method_enum != http_common::REQUEST_METHOD::POST)
    {
      throw BadRequest("Only POST is accepted");
    }
  }

  int req_len = 0;
  int ret = sscanf(req_util.CONTENT_LENGTH, "%d", &req_len);
  if(ret != 1)
  {
    throw BadRequest("Could not parse CONTENT_LENGTH");
  }

  if((req_len < 0) || (req_len > MAX_REQ_LEN))
  {
    throw BadRequest("CONTENT_LENGTH is invalid");
  }

  //validate CONTENT_TYPE, ignoring any optional charset
  { 
    const char app_jsonrpc[] = "application/json";
    if(strlen(req_util.CONTENT_TYPE) == 0)
    {
      throw BadRequest("CONTENT_TYPE is invalid");
    }

    if(strncmp(req_util.CONTENT_TYPE, app_jsonrpc, sizeof(app_jsonrpc)-1) != 0)
    {
      throw BadRequest("CONTENT_TYPE is invalid");
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
      m_req_str.assign(m_req_buf.begin(), m_req_buf.end());
      SPDLOG_INFO("Request: {:.{}}",  m_req_buf.data(), m_req_buf.size());
    }

    // it wants CONTENT_TYPE == application/json - the application/json; charset=UTF-8 causes it to fail
    // std::shared_ptr<jsonrpc::FormattedData> result = m_jsonrpc_server_ptr->HandleRequest(m_req_str, CONTENT_TYPE);
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
    SPDLOG_INFO("Response: {:.{}}", result_str.data(), result_str.size());
  }
  else
  {
    SPDLOG_ERROR("Response: Empty");
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
  FCGX_Finish_r(request);
}
