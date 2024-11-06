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

#define NO_FCGI_DEFINES 1
#include <fcgi_config.h>
#include <fcgiapp.h>

#include <functional>
#include <memory>
#include <vector>

class http_req_jpeg : public http_req_callback_base
{
public:

  http_req_jpeg()
  {
    
  }

  ~http_req_jpeg() override
  {
    
  }

  void handle(FCGX_Request* const request) override;

  typedef std::function<void(std::vector<uint8_t>*)> GetImageCb;
  void set_get_image_cb(const GetImageCb& cb)
  {
    m_cb = cb;
  }

protected:

  GetImageCb m_cb;

  std::shared_ptr<std::vector<uint8_t>> m_buf;

};
