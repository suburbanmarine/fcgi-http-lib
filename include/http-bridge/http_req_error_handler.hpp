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

#include "http-bridge/http_common.hpp"

#define NO_FCGI_DEFINES 1
#include <fcgi_config.h>
#include <fcgiapp.h>

#include <array>

class http_req_error_handler : public http_req_callback_base
{
public:

  http_req_error_handler()
  {
    m_status_code = http_common::STATUS_CODES::OK;
  }

  ~http_req_error_handler() override
  {
    
  }

  void handle(FCGX_Request* const request) override;

protected:

http_common::STATUS_CODES m_status_code;

};
