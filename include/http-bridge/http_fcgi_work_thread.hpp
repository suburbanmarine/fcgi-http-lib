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

#include "http-bridge/http_fcgi_svr_fwd.hpp"

#include "emb-lin-util/Thread_base.hpp"

class http_fcgi_work_thread : public Thread_base
{
public:

  http_fcgi_work_thread();
  ~http_fcgi_work_thread();

  bool init(http_fcgi_svr* svr, int sock_fd);

  void work() override;

protected:
  http_fcgi_svr* m_svr;
  int m_sock_fd;
};
