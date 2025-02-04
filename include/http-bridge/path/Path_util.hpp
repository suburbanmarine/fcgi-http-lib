/**
 * This file is part of fcgi-http-lib, a library with utilities for handling basic HTTP requests over FCGI.
 * 
 * This software is distrubuted in the hope it will be useful, but without any warranty, including the implied warrranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See LICENSE.txt for details.
 * 
 * @author Jacob Schloss <jacob.schloss@suburbanmarine.io>
 * @copyright Copyright (c) 2023, 2025 Suburban Marine, Inc. All rights reserved.
 * @license Licensed under the LGPL-3.0 license. See LICENSE.txt for details.
 * SPDX-License-Identifier: LGPL-3.0-only
*/

#pragma once

#include <filesystem>

#include <cstddef>

class Path_util
{
public:
	static size_t num_elements(const std::filesystem::path& path);

	static bool trailing_element_is_dir(const std::filesystem::path& path);

	// returns true if base_path is a parent path or identical to req_path
	static bool is_parent_path(const std::filesystem::path& base_path, const std::filesystem::path& req_path);
};
