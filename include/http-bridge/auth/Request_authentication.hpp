#pragma once

/**
 * @author Jacob Schloss <jacob.schloss@suburbanmarine.io>
 * @copyright Copyright (c) 2024 Suburban Marine, Inc. All rights reserved.
 * @license Licensed under the 3-Clause BSD LICENSE. See LICENSE.txt for details.
*/

#include "http-bridge/auth/Request_authentication.hpp"

#define NO_FCGI_DEFINES 1
#include <fcgi_config.h>
#include <fcgiapp.h>

// https://datatracker.ietf.org/doc/rfc9421/

// Content-Digest: <digest-algorithm>=:<standard-padded-base64-digest-value>:
// digest-algorithm in [sha-256]

class Request_authentication
{
public:

	static std::string make_canonical_header(FCGX_Request* const request);

	static std::string make_canonical_header_string(FCGX_Request* const request);

	const std::string to_lower_trim(const std::string& str);
	const std::string to_upper_trim(const std::string& str);

protected:

	std::map< std::string, std::vector<char> > m_client_pubkeys;
};
