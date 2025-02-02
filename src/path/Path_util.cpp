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

#include "http-bridge/path/Path_util.hpp"
// #include <iostream>

size_t Path_util::num_elements(const std::filesystem::path& path)
{
	return std::distance(path.begin(), path.end());
}

bool Path_util::trailing_element_is_dir(const std::filesystem::path& path)
{
	if(path.empty())
	{
		return true;
	}

	const std::string last = std::prev(path.end())->string();
	
	// std::cout << "path: '" << path << "' end: '" << last << "'" << std::endl;
	
	return (last == ".") || (last == "/") || (last.empty());
}

// returns true if base_path is a parent path or identical to req_path
bool Path_util::is_parent_path(const std::filesystem::path& base_path, const std::filesystem::path& req_path)
{
	if( base_path.empty() )
	{
		throw std::domain_error("path must not be empty");
	}

	if( req_path.empty() )
	{
		throw std::domain_error("path must not be empty");
	}

	if( ! base_path.is_absolute() )
	{
		throw std::domain_error("must be absolute path");
	}

	if( ! req_path.is_absolute() )
	{
		throw std::domain_error("must be absolute path");
	}

	const std::filesystem::path normal_base_path = base_path.lexically_normal();
	const std::filesystem::path normal_req_path  = req_path.lexically_normal();

	const std::filesystem::path prox_path = normal_req_path.lexically_proximate(normal_base_path);

	// std::cout << "base_path: " << base_path << std::endl;
	// std::cout << "req_path: "  << req_path  << std::endl;
	// std::cout << "normal_base_path: " << normal_base_path << std::endl;
	// std::cout << "normal_req_path: "  << normal_req_path  << std::endl;
	// std::cout << "prox_path: "  << prox_path  << std::endl;
	// std::cout << std::endl;

	if( prox_path.empty() )
	{
		return true;
	}

	if(prox_path.begin()->string() == "..")
	{
		return false;
	}

	return true;
}
