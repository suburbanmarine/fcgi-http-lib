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

#include <boost/filesystem/path.hpp>

#include <cstddef>

class Path_util
{
public:
	static size_t num_elements(const boost::filesystem::path& path);

	static bool trailing_element_is_dir(const boost::filesystem::path& path);

	// returns true if base_path is a parent path or identical to req_path
	static bool is_parent_path(const boost::filesystem::path& base_path, const boost::filesystem::path& req_path);
};
