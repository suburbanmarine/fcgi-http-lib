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

#include "http-bridge/http_req_error_handler.hpp"

void http_req_error_handler::handle(FCGX_Request* const request)
{
  const char* code_msg = http_common::get_status_code_str(m_status_code);

  FCGX_PutS("Content-Type: text/html\r\n", request->out);
  FCGX_FPrintF(request->out, "Status: {:d} {:s}\r\n", 
      m_status_code,
      code_msg
    );
  FCGX_PutS("\r\n", request->out);
  FCGX_PutS(code_msg, request->out);

  FCGX_Finish_r(request);
}
