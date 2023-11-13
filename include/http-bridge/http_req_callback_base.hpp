/**
 * @author Jacob Schloss <jacob.schloss@suburbanmarine.io>
 * @copyright Copyright (c) 2021 Suburban Marine, Inc. All rights reserved.
 * @license Licensed under the 3-Clause BSD LICENSE. See LICENSE.txt for details.
*/

#pragma once

#define NO_FCGI_DEFINES 1
#include <fcgi_config.h>
#include <fcgiapp.h>

#include <stdexcept>

class HTTPException : public std::runtime_error
{
public:
  HTTPException(const int code, const char msg[]);
  int get_code() const;

protected:
  const int m_code;
};

class BadRequest : public HTTPException
{
  public:
  BadRequest();
  BadRequest(const char msg[]);
};

class NotFound : public HTTPException
{
  public:
  NotFound();
  NotFound(const char msg[]);
};

class MethodNotAllowed : public HTTPException
{
  public:
  MethodNotAllowed();
  MethodNotAllowed(const char msg[]);
};

class PayloadTooLarge : public HTTPException
{
  public:
  PayloadTooLarge();
  PayloadTooLarge(const char msg[]);
};

class URITooLong : public HTTPException
{
  public:
  URITooLong();
  URITooLong(const char msg[]);
};


class RequestHeaderFieldsTooLarge : public HTTPException
{
  public:
  RequestHeaderFieldsTooLarge();
  RequestHeaderFieldsTooLarge(const char msg[]);
};

class InternalServerError : public HTTPException
{
  public:
  InternalServerError();
  InternalServerError(const char msg[]);
};

class ServiceUnavailable : public HTTPException
{
  public:
  ServiceUnavailable();
  ServiceUnavailable(const char msg[]);
};

class http_req_callback_base
{
public:

  http_req_callback_base();

  virtual ~http_req_callback_base();

  virtual void handle(FCGX_Request* const request);

protected:

};
