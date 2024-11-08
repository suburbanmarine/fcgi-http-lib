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

#include "http-bridge/http_req_callback_base.hpp"

#include <http-bridge/http_common.hpp>

#include <exception>

HTTPException::HTTPException(const int code, const char msg[]) : std::runtime_error(msg), m_code(code)
{

}
int HTTPException::get_code() const
{
    return m_code;
}

BadRequest::BadRequest() : HTTPException(http_common::STATUS_CODES::BAD_REQUEST, "Bad Request")
{

}
BadRequest::BadRequest(const char msg[]) : HTTPException(http_common::STATUS_CODES::BAD_REQUEST, msg)
{

}

NotFound::NotFound() : HTTPException(http_common::STATUS_CODES::NOT_FOUND, "Not Found")
{

}
NotFound::NotFound(const char msg[]) : HTTPException(http_common::STATUS_CODES::NOT_FOUND, msg)
{

}

MethodNotAllowed::MethodNotAllowed() : HTTPException(http_common::STATUS_CODES::METHOD_NOT_ALLOWED, "Method Not Allowed")
{

}
MethodNotAllowed::MethodNotAllowed(const char msg[]) : HTTPException(http_common::STATUS_CODES::METHOD_NOT_ALLOWED, msg)
{

}

PayloadTooLarge::PayloadTooLarge() : HTTPException(http_common::STATUS_CODES::PAYLOAD_TOO_LARGE, "Payload Too Large")
{

}
PayloadTooLarge::PayloadTooLarge(const char msg[]) : HTTPException(http_common::STATUS_CODES::PAYLOAD_TOO_LARGE, msg)
{

}

URITooLong::URITooLong() : HTTPException(http_common::STATUS_CODES::URI_TOO_LONG, "URI Too Long")
{

}
URITooLong::URITooLong(const char msg[]) : HTTPException(http_common::STATUS_CODES::URI_TOO_LONG, msg)
{

}

UnsupportedMediaType::UnsupportedMediaType() : HTTPException(http_common::STATUS_CODES::UNSUPPORTED_MEDIA_TYPE, "Unsupported Media Type")
{

}
UnsupportedMediaType::UnsupportedMediaType(const char msg[]) : HTTPException(http_common::STATUS_CODES::UNSUPPORTED_MEDIA_TYPE, msg)
{

}

RequestHeaderFieldsTooLarge::RequestHeaderFieldsTooLarge() : HTTPException(http_common::STATUS_CODES::REQUEST_HEADER_FIELDS, "Request Header Fields Too Large")
{

}
RequestHeaderFieldsTooLarge::RequestHeaderFieldsTooLarge(const char msg[]) : HTTPException(http_common::STATUS_CODES::REQUEST_HEADER_FIELDS, msg)
{

}

InternalServerError::InternalServerError() : HTTPException(http_common::STATUS_CODES::INTERNAL_SERVER_ERROR, "Internal Error")
{

}
InternalServerError::InternalServerError(const char msg[]) : HTTPException(http_common::STATUS_CODES::INTERNAL_SERVER_ERROR, msg)
{

}

ServiceUnavailable::ServiceUnavailable() : HTTPException(http_common::STATUS_CODES::SERVICE_UNAVAILABLE, "Service Unavailable")
{

}
ServiceUnavailable::ServiceUnavailable(const char msg[]) : HTTPException(http_common::STATUS_CODES::SERVICE_UNAVAILABLE, msg)
{

}

http_req_callback_base::http_req_callback_base()
{

}

http_req_callback_base::~http_req_callback_base()
{

}

void http_req_callback_base::handle(FCGX_Request* const request)
{
    FCGX_PutS("Status: 500 Internal Error\r\n", request->out);
    FCGX_Finish_r(request);
}