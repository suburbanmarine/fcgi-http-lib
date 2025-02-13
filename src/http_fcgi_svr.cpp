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

#include "http-bridge/http_fcgi_svr.hpp"

#include <spdlog/spdlog.h>

http_fcgi_svr::http_fcgi_svr()
{
  m_sock_fd = -1;
}

http_fcgi_svr::~http_fcgi_svr()
{
  if( ! m_thread_pool.empty() )
  {
    stop();
  }
}

bool http_fcgi_svr::start(const char* bind_addr, const size_t num_threads)
{
  return start(bind_addr, num_threads, 256);
}

bool http_fcgi_svr::start(const char* bind_addr, const size_t num_threads, const int backlog)
{
  SPDLOG_INFO("Starting server");

  SPDLOG_INFO("Binding to {:s}", bind_addr);
  m_sock_fd = FCGX_OpenSocket(bind_addr, backlog);

  if(m_sock_fd < 0)
  {
    SPDLOG_ERROR("Server failed to open socket"); 
    return false;
  }
 
  SPDLOG_INFO("FCGX_Init");
  FCGX_Init();

  SPDLOG_INFO("Staring threads");
  m_thread_pool.resize(num_threads);
  for(size_t i = 0; i < m_thread_pool.size(); i++)
  {
    m_thread_pool[i] = std::make_shared<http_fcgi_work_thread>();
    if( ! m_thread_pool[i]->init(this, m_sock_fd) )
    {
      SPDLOG_INFO("Thread init failed");
      return false;
    }
    m_thread_pool[i]->launch();
  }

  SPDLOG_INFO("Server started");
  return true;
}

bool http_fcgi_svr::stop()
{
  bool fret = true;
  SPDLOG_INFO("Stopping server");

  //interrupt all threads
  for(size_t i = 0; i < m_thread_pool.size(); i++)
  {
    m_thread_pool[i]->interrupt();
  }

  //shutdown FCGI
  FCGX_ShutdownPending();

  //close socket
  if(m_sock_fd != -1)
  {
    int ret = shutdown(m_sock_fd, SHUT_RDWR);
    if(ret != 0)
    {
      SPDLOG_ERROR("Error shutting down socket: {:d}", errno);
      fret = false;
    }

    ret = close(m_sock_fd);
    if(ret != 0)
    {
      SPDLOG_ERROR("Error closing socket: {:d}", errno);
      fret = false;
    }
    m_sock_fd = -1;
  }

  //join all threads
  for(size_t i = 0; i < m_thread_pool.size(); i++)
  {
    m_thread_pool[i]->join();
    m_thread_pool[i].reset();
  }
  m_thread_pool.clear();

  SPDLOG_INFO("Server stopped");

  return fret;
}

void http_fcgi_svr::register_cb_for_doc_uri(const char* doc_uri, const std::shared_ptr<http_req_callback_base>& cb)
{
  m_cb_table.set_node(doc_uri, std::make_shared<http_fcgi_svr_cb>(cb));
}
std::shared_ptr<http_req_callback_base> http_fcgi_svr::get_cb_for_doc_uri(const char* doc_uri)
{
  Directory_tree_node::ptr node = m_cb_table.find_match(doc_uri, Directory_tree::MATCH_TYPE::EXACT);
  if( ! node )
  {
    node = m_cb_table.find_match(doc_uri, Directory_tree::MATCH_TYPE::PARENT_PATH);
  }

  std::shared_ptr<http_req_callback_base> cb;
  if(node)
  {
    auto cb_ptr = node->get_data<http_fcgi_svr_cb>();
    if(cb_ptr)
    {
      cb = cb_ptr->m_cb;
    }
  }

  return cb;
}

std::shared_ptr<spdlog::logger> http_fcgi_svr::create_logger(const std::string& logger_name, const std::vector<spdlog::sink_ptr>& sinks)
{
  auto logger = spdlog::get(logger_name);
  if( ! logger )
  {
    logger = std::make_shared<spdlog::logger>(logger_name, std::begin(sinks), std::end(sinks));
    logger->set_level(spdlog::level::debug);
    spdlog::set_default_logger( logger );
  }

  return logger;
}

void http_fcgi_svr::register_logger(const std::shared_ptr<spdlog::logger>& logger)
{
  spdlog::register_logger(logger);
  spdlog::set_default_logger(logger);
}