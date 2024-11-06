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

#include "http-bridge/http_req_callback_base_fwd.hpp"
#include "http-bridge/http_fcgi_work_thread.hpp"

#include "http-bridge/path/Directory_tree.hpp"

#include <spdlog/spdlog.h>

#define NO_FCGI_DEFINES 1
#include <fcgi_config.h>
#include <fcgiapp.h>

#include <sys/socket.h>
#include <unistd.h>

#include <map>
#include <memory>
#include <vector>

class http_fcgi_svr_cb : public Directory_tree_node::Data
{
public:
  
  http_fcgi_svr_cb(const std::shared_ptr<http_req_callback_base>& cb) : m_cb(cb)
  {

  }

  std::shared_ptr<http_req_callback_base> m_cb;
};

class http_fcgi_svr
{
public:

  http_fcgi_svr();

  ~http_fcgi_svr();

  bool start(const char* bind_addr, const size_t num_threads);
  bool start(const char* bind_addr, const size_t num_threads, const int backlog);

  bool stop();

  void register_cb_for_doc_uri(const char* doc_uri, const std::shared_ptr<http_req_callback_base>& cb);
  std::shared_ptr<http_req_callback_base> get_cb_for_doc_uri(const char* doc_uri);

  // NOT MT safe - call before using library
  static std::shared_ptr<spdlog::logger> create_logger(const std::string& logger_name, const std::vector<spdlog::sink_ptr>& sinks);

  // NOT MT safe - call before using library
  static void register_logger(const std::shared_ptr<spdlog::logger>& logger);

protected:
  int m_sock_fd;

  std::vector<std::shared_ptr<http_fcgi_work_thread>> m_thread_pool;

  // exact match
  // map doc uri to callback handler
  // std::map<std::string, std::shared_ptr<http_req_callback_base>> m_cb_table;
  Directory_tree m_cb_table;

  // parent dir match
  // TODO: map dir prefix to cb
  // std::map<std::string, std::shared_ptr<http_req_callback_base>> m_cb_parent_table;

  // regex match
  // TODO: map regex to cb 
  // std::list<std::shared_ptr<boost::regex>, std::shared_ptr<http_req_callback_base>> m_cb_regex_table;
};
