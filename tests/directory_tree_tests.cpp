/**
 * This file is part of fcgi-http-lib, a library with utilities for handling basic HTTP requests over FCGI.
 * 
 * This software is distrubuted in the hope it will be useful, but without any warranty, including the implied warrranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See LICENSE.txt for details.
 * 
 * @author Jacob Schloss <jacob.schloss@suburbanmarine.io>
 * @copyright Copyright (c) 2025 Suburban Marine, Inc. All rights reserved.
 * @license Licensed under the LGPL-3.0 license. See LICENSE.txt for details.
 * SPDX-License-Identifier: LGPL-3.0-only
*/

#include "http-bridge/path/Directory_tree.hpp"

#include <gtest/gtest.h>

class http_req_callback : public Directory_tree_node::Data
{

};

TEST(Directory_tree, register_cb)
{
	Directory_tree cb_table;

	auto handler1 = std::make_shared<http_req_callback>();
	auto handler2 = std::make_shared<http_req_callback>();

	EXPECT_NE(
		cb_table.set_node("/", 
			handler1
		),
		nullptr
	);

	EXPECT_NE(
		cb_table.set_node("/api/v1", 
			handler2
		),
		nullptr
	);

	Directory_tree_node::ptr match = cb_table.find_match("/api/v1", Directory_tree::MATCH_TYPE::EXACT);
	EXPECT_NE(match, nullptr);
	EXPECT_EQ(match->get_data<http_req_callback>(), handler2);
	
	match = cb_table.find_match("/api/v1/foo/bar/baz", Directory_tree::MATCH_TYPE::PARENT_PATH);
	EXPECT_NE(match, nullptr);
	if(match)
	{
		EXPECT_EQ(match->get_data<http_req_callback>(), handler2);
	}

	match = cb_table.find_match("/api/", Directory_tree::MATCH_TYPE::PARENT_PATH);
	EXPECT_NE(match, nullptr);
	if(match)
	{
		EXPECT_EQ(match->get_data<http_req_callback>(), handler1);
	}

	match = cb_table.find_match("/api", Directory_tree::MATCH_TYPE::PARENT_PATH);
	EXPECT_NE(match, nullptr);
	if(match)
	{
		EXPECT_EQ(match->get_data<http_req_callback>(), handler1);
	}

	match = cb_table.find_match("/", Directory_tree::MATCH_TYPE::PARENT_PATH);
	EXPECT_NE(match, nullptr);
	if(match)
	{
		EXPECT_EQ(match->get_data<http_req_callback>(), handler1);
	}

	EXPECT_EQ(cb_table.find_match("/api/v", Directory_tree::MATCH_TYPE::EXACT), nullptr);
	EXPECT_EQ(cb_table.find_match("/api/", Directory_tree::MATCH_TYPE::EXACT),  nullptr);
	EXPECT_EQ(cb_table.find_match("/api", Directory_tree::MATCH_TYPE::EXACT),   nullptr);
	EXPECT_EQ(cb_table.find_match("/ap", Directory_tree::MATCH_TYPE::EXACT),    nullptr);

	match = cb_table.find_match("/", Directory_tree::MATCH_TYPE::EXACT);
	EXPECT_NE(match, nullptr);
	if(match)
	{
		EXPECT_EQ(match->get_data<http_req_callback>(), handler1);
	}


	EXPECT_THROW(cb_table.find_match("", Directory_tree::MATCH_TYPE::EXACT),    std::domain_error);
}
