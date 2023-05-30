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
