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

#include "http-bridge/path/Directory_tree_node.hpp"

#include <sstream>
#include <ostream>

void Directory_tree_node::to_stream(std::ostream& os) const
{
	// size_t child_count = m_root->get_child_count();
	// for(size_t i = 0; i < child_count; i++)
	// {
	// 	Directory_tree_node::ptr curr_node = get_child_by_idx
	// }
}

std::string Directory_tree_node::to_string() const
{
	std::stringstream ss;
	to_stream(ss);
	return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Directory_tree_node& rhs)
{
	rhs.to_stream(os);
	return os;
}
