/**
 * @author Jacob Schloss <jacob.schloss@suburbanmarine.io>
 * @copyright Copyright (c) 2024 Suburban Marine, Inc. All rights reserved.
 * @license Licensed under the 3-Clause BSD LICENSE. See LICENSE.txt for details.
*/

#include "http-bridge/auth/Request_authentication.hpp"

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/trim.hpp>

const std::string Request_authentication::to_lower_trim(const std::string& str)
{
	std::string temp = str;

	boost::algorithm::trim(temp);
	boost::algorithm::to_lower(temp)

	return std::move(temp);
}

const std::string Request_authentication::to_upper_trim(const std::string& str)
{
	std::string temp = str;

	boost::algorithm::trim(temp);
	boost::algorithm::to_upper(temp)

	return std::move(temp);
}

static std::string Request_authentication::make_canonical_header_string(FCGX_Request* const request)
{
	std::map<std::string, std::string> headers;

	headers["content-type"]   = boost::algorithm::trim_copy(req.CONTENT_TYPE);
	headers["content-digest"] = boost::algorithm::trim_copy(FCGX_GetParam(request->envp, "CONTENT_DIGEST"));
	headers["host"]           = boost::algorithm::trim_copy(req.host);
	headers["x-sm-date"]      = boost::algorithm::trim_copy(FCGX_GetParam(request->envp, "X_SM_DATE"));
	headers["x-sm-key-id"]    = boost::algorithm::trim_copy(FCGX_GetParam(request->envp, "X_SM_SIG_KEY_ID"));
	headers["x-sm-key-algo"]  = boost::algorithm::trim_copy(FCGX_GetParam(request->envp, "X_SM_SIG_KEY_ALGO"));

	std::string temp;
	for(const auto val& : headers)
	{
		if( ! val.second.empty() )
		{
			temp += val.first;
			temp += ":";
			temp += val.second;
			temp += "\n";
		}
	}
}

std::string Request_authentication::make_canonical_request_string(FCGX_Request* const request)
{
	http_req_util req;

	std::string REQUEST_METHOD = to_upper_trim(req.REQUEST_METHOD);             // cannonical
	std::string DOCUMENT_URI   = req.DOCUMENT_URI;                              // URI encoded
	std::string QUERY_STRING   = make_canonical_query_string(req.QUERY_STRING); // URI encoded, sorted alphabetically by name

	// Cannonical Headers, header lower case
	std::string canonical_headers = make_canonical_header_string(request);
}