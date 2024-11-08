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

#pragma once

#include "http-bridge/http_req_callback_base.hpp"
#include "http-bridge/http_req_jsonrpc_fwd.hpp"
#include "http-bridge/http_req_util_fwd.hpp"

#define NO_FCGI_DEFINES 1
#include <fcgi_config.h>
#include <fcgiapp.h>

#include <memory>
#include <string>
#include <vector>
#include <mutex>

class http_req_jsonrpc : public http_req_callback_base
{
public:

  http_req_jsonrpc()
  {
    m_req_str.reserve(MAX_REQ_LEN);
    m_req_buf.reserve(MAX_REQ_LEN);
  }

  ~http_req_jsonrpc() override
  {
    
  }

  void handle(FCGX_Request* const request) override;

  void set_rpc_server(const std::shared_ptr<jsonrpccxx::JsonRpc2Server>& server)
  {
    m_jsonrpc_server_ptr = server;
  }

protected:

  void handle_req_option(FCGX_Request* const request, const http_req_util& req_util);
  void handle_req_post(FCGX_Request* const request, const http_req_util& req_util);

  static constexpr int MAX_REQ_LEN = 16*1024;

  std::mutex m_jsonrpc_mutex;

  //idk if this is thread safe
  std::shared_ptr<jsonrpccxx::JsonRpc2Server> m_jsonrpc_server_ptr;

  //these need to be thread local
  std::vector<char> m_req_buf;
  std::string m_req_str;
};
