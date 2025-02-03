#include "http-bridge/path/Path_util.hpp"

#include <gtest/gtest.h>

TEST(Path_util, num_elements)
{
	EXPECT_EQ(Path_util::num_elements(""),             0U);
	EXPECT_EQ(Path_util::num_elements("/"),            1U);
	EXPECT_EQ(Path_util::num_elements("/foo"),         2U);
	EXPECT_EQ(Path_util::num_elements("/foo/"),        3U);
	EXPECT_EQ(Path_util::num_elements("/foo/bar/baz"), 4U);
}

TEST(Path_util, trailing_element_is_dir)
{
	EXPECT_TRUE(Path_util::trailing_element_is_dir(""));
	EXPECT_TRUE(Path_util::trailing_element_is_dir("/"));
	EXPECT_TRUE(Path_util::trailing_element_is_dir("/."));

	EXPECT_TRUE(Path_util::trailing_element_is_dir("/foo/.."));
	EXPECT_TRUE(Path_util::trailing_element_is_dir("/foo/bar/baz/"));
	EXPECT_TRUE(Path_util::trailing_element_is_dir("/foo/bar/baz/."));
	EXPECT_FALSE(Path_util::trailing_element_is_dir("/foo/bar/baz"));
}

TEST(Path_util, is_parent_path)
{
	EXPECT_TRUE(Path_util::is_parent_path("/", "/foo"));
	EXPECT_TRUE(Path_util::is_parent_path("/", "/foo/bar"));

	EXPECT_TRUE(Path_util::is_parent_path("/", "/"));
	EXPECT_TRUE(Path_util::is_parent_path("/foo", "/foo"));
	EXPECT_TRUE(Path_util::is_parent_path("/foo/", "/foo/"));

	EXPECT_TRUE(Path_util::is_parent_path("/", "/foo/bar/baz"));
	EXPECT_TRUE(Path_util::is_parent_path("/foo", "/foo/bar/baz"));
	EXPECT_TRUE(Path_util::is_parent_path("/foo/", "/foo/bar/baz"));
	EXPECT_TRUE(Path_util::is_parent_path("/foo/bar", "/foo/bar/baz"));

	EXPECT_FALSE(Path_util::is_parent_path("/foo/1/baz", "/foo/bar/work"));
	EXPECT_FALSE(Path_util::is_parent_path("/foo/1/baz", "/foo/work"));
	EXPECT_FALSE(Path_util::is_parent_path("/foo/1/baz", "/foo/2/baz"));

	EXPECT_FALSE(Path_util::is_parent_path("/foo/bar/baz", "/foo/bar/../work"));
	EXPECT_TRUE(Path_util::is_parent_path("/foo/bar/baz", "/foo/bar/baz/foo/../../baz/work"));

	EXPECT_TRUE(Path_util::is_parent_path("/foo/work/../baz", "/foo/baz/bar"));
}
