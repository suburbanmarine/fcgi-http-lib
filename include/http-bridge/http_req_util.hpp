/**
 * This file is part of fcgi-http-lib, a library with utilities for handling basic HTTP requests over FCGI.
 * 
 * This software is distrubuted in the hope it will be useful, but without any warranty, including the implied warrranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See LICENSE.txt for details.
 * 
 * @author Jacob Schloss <jacob.schloss@suburbanmarine.io>
 * @copyright Copyright (c) 2023 Suburban Marine, Inc. All rights reserved.
 * @license Licensed under the LGPL-3.0 license. See LICENSE.txt for details.
 * SPDX-License-Identifier: LGPL-3.0-only
*/

#pragma once

#include "http-bridge/http_common.hpp"


#define NO_FCGI_DEFINES 1
#include <fcgi_config.h>
#include <fcgiapp.h>

#include <filesystem>
#include <map>
#include <optional>
#include <string>

class http_req_util
{
public:
	http_req_util();
	~http_req_util();

	void load(FCGX_Request* const request);

	void log_request_env();

	typedef std::map<std::string, std::optional<std::string>> Query_map;
	bool parse_query_string(Query_map* const out_query_map);

	std::filesystem::path doc_uri_path;
	http_common::REQUEST_METHOD request_method_enum;

	char const * QUERY_STRING;
	char const * REQUEST_METHOD;
	char const * CONTENT_TYPE;
	char const * CONTENT_LENGTH;
	char const * SCRIPT_NAME;
	char const * REQUEST_URI;
	char const * DOCUMENT_URI;
	char const * DOCUMENT_ROOT;
	char const * SERVER_PROTOCOL;
	char const * REQUEST_SCHEME;
	char const * HTTPS;
	char const * GATEWAY_INTERFACE;
	char const * SERVER_SOFTWARE;
	char const * REMOTE_ADDR;
	char const * REMOTE_PORT;
	char const * SERVER_ADDR;
	char const * SERVER_PORT;
	char const * SERVER_NAME;
	char const * REDIRECT_STATUS;
};
