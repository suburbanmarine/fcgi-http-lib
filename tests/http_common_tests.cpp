#include "http-bridge/http_common.hpp"

#include <gtest/gtest.h>

TEST(parse_content_type, basic_mime)
{
	http_common::Content_type type;
	ASSERT_TRUE(http_common::parse_content_type("application/json", &type));

	ASSERT_STREQ(type.media_type.c_str(), "application/json");
	ASSERT_TRUE(type.charset.empty());
	ASSERT_TRUE(type.boundary.empty());
}

TEST(parse_content_type, charset)
{
	http_common::Content_type type;
	ASSERT_TRUE(http_common::parse_content_type("application/json; charset=utf-8", &type));

	ASSERT_STREQ(type.media_type.c_str(), "application/json");
	ASSERT_STREQ(type.charset.c_str(), "utf-8");
	ASSERT_TRUE(type.boundary.empty());
}

TEST(parse_content_type, space_reject)
{
	http_common::Content_type type;
	ASSERT_TRUE(http_common::parse_content_type(" \tapplication/json ; \tcharset=utf-8 \t;\tboundary=foo ", &type));

	ASSERT_STREQ(type.media_type.c_str(), "application/json");
	ASSERT_STREQ(type.charset.c_str(), "utf-8");
	ASSERT_STREQ(type.boundary.c_str(), "foo");
}

TEST(parse_content_type, boundary)
{
	http_common::Content_type type;
	ASSERT_TRUE(http_common::parse_content_type("application/json; boundary=foo", &type));

	ASSERT_STREQ(type.media_type.c_str(), "application/json");
	ASSERT_TRUE(type.charset.empty());
	ASSERT_STREQ(type.boundary.c_str(), "foo");
}

TEST(parse_content_type, ignore_invalid)
{
	{
		http_common::Content_type type;
		ASSERT_TRUE(http_common::parse_content_type("application/json;", &type));
		ASSERT_STREQ(type.media_type.c_str(), "application/json");
		ASSERT_TRUE(type.charset.empty());
		ASSERT_TRUE(type.boundary.empty());
	}
	{
		http_common::Content_type type;
		ASSERT_TRUE(http_common::parse_content_type("application/json;foo=bar", &type));
		ASSERT_STREQ(type.media_type.c_str(), "application/json");
		ASSERT_TRUE(type.charset.empty());
		ASSERT_TRUE(type.boundary.empty());
	}
	{
		http_common::Content_type type;
		ASSERT_TRUE(http_common::parse_content_type("application/json; foo=bar; charset=utf-8; baz=44; boundary=foo", &type));
	
		ASSERT_STREQ(type.media_type.c_str(), "application/json");
		ASSERT_STREQ(type.charset.c_str(), "utf-8");
		ASSERT_STREQ(type.boundary.c_str(), "foo");
	}
}