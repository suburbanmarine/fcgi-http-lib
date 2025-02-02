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

#include "Directory_tree_fwd.hpp"
#include "Directory_tree_node.hpp"

#include <filesystem>
#include <iosfwd>
#include <string>

class Directory_tree
{
public:

	Directory_tree()
	{
		m_root = Directory_tree_node::create(Directory_tree_node::ptr(), "/");
	}

	virtual ~Directory_tree()
	{

	}

	enum MATCH_TYPE
	{
		PARENT_PATH,
		EXACT,
		NONE
	};

	Directory_tree_node::ptr set_node(const std::filesystem::path& full_path)
	{
		return set_node(full_path, Directory_tree_node::Data::ptr());
	}

	/// if node matches path, set node to this handler
	/// if node does not exist, create a new node
	Directory_tree_node::ptr set_node(const std::filesystem::path& full_path, const Directory_tree_node::Data::ptr& data);

	Directory_tree_node::ptr find_match(const std::filesystem::path& query_path, const MATCH_TYPE mode = MATCH_TYPE::EXACT);

	bool has_children() const
	{
		return m_root->has_children();
	}

	void to_stream(std::ostream& os) const;
	std::string to_string() const;

protected:

	Directory_tree_node::ptr m_root;
};

std::ostream& operator<<(std::ostream& os, const Directory_tree& rhs);
