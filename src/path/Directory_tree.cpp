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

#include "http-bridge/path/Directory_tree.hpp"

#include <sstream>
#include <ostream>

Directory_tree_node::ptr Directory_tree::set_node(const boost::filesystem::path& full_path, const Directory_tree_node::Data::ptr& data)
{
	if( ! full_path.is_absolute() )
	{
		throw std::domain_error("full_path must be absolute path");
	}

	if( ! m_root->is_node_name(full_path.begin()->string()) )
	{
		return Directory_tree_node::ptr();
	}

	Directory_tree_node::ptr curr_node = m_root;
	boost::filesystem::path curr_path  = m_root->path();

	for(auto q_it = std::next(full_path.begin()); q_it != full_path.end(); ++q_it)
	{
		curr_path = curr_path / *q_it;
			
		Directory_tree_node::ptr child = curr_node->get_child_by_name(q_it->string());
		if( ! child )
		{
			child = curr_node->create_child(curr_path);
		}

		curr_node = child;
	}

	if(curr_node)
	{
		curr_node->set_data(data);
	}

	return curr_node;
}

Directory_tree_node::ptr Directory_tree::find_match(const boost::filesystem::path& query_path, const MATCH_TYPE mode)
{
	if( ! query_path.is_absolute() )
	{
		throw std::domain_error("query_path must be absolute path");
	}

	if( ! m_root->is_node_name(query_path.begin()->string()) )
	{
		return Directory_tree_node::ptr();
	}

	Directory_tree_node::ptr curr_node = m_root;
	for(auto q_it = std::next(query_path.begin()); q_it != query_path.end(); ++q_it)
	{
		Directory_tree_node::ptr next_node = curr_node->get_child_by_name(q_it->string());
		if( ! next_node )
		{
			if(mode == MATCH_TYPE::PARENT_PATH)
			{
				curr_node = curr_node->get_child_by_name(".");
			}
			break;
		}
		curr_node = next_node;
	}

	if(curr_node)
	{
		switch(mode)
		{
			case MATCH_TYPE::PARENT_PATH:
			{
				//exact match ok
				if(curr_node->is_path(query_path))
				{
					return curr_node;
				}
				//check curr_node is dir
				if(Path_util::trailing_element_is_dir(curr_node->m_full_path))
				{
					return curr_node;
				}
				break;
			}
			case MATCH_TYPE::EXACT:
			{
				//check exact match
				if(curr_node->is_path(query_path))
				{
					return curr_node;
				}
				break;
			}
			default:
			{
				throw std::domain_error("mode is oob");
				break;
			}
		}
	}

	//no match
	return Directory_tree_node::ptr();
}

void Directory_tree::to_stream(std::ostream& os) const
{
	// size_t child_count = m_root->get_child_count();
	// for(size_t i = 0; i < child_count; i++)
	// {
	// 	Directory_tree_node::ptr curr_node = get_child_by_idx
	// }
}

std::string Directory_tree::to_string() const
{
	std::stringstream ss;
	to_stream(ss);
	return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Directory_tree& rhs)
{
	rhs.to_stream(os);
	return os;
}
