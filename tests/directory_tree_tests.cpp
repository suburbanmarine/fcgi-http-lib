#include "http-bridge/path/Directory_tree.hpp"

#include <gtest/gtest.h>

class http_req_callback : public Directory_tree_node::Data
{

};

TEST(Directory_tree, register_cb)
{
	Directory_tree cb_table;

	EXPECT_NE(
		cb_table.set_node("/api/v1", 
			std::make_shared<http_req_callback>()
		),
		nullptr
	);

	EXPECT_NE(cb_table.find_match("/api/v1", Directory_tree::MATCH_TYPE::EXACT), nullptr);
}
