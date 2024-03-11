project(fcgi-http-svr
	VERSION 0.1.0
	DESCRIPTION "FastCGI utility library for C++"
	LANGUAGES C CXX
)

add_library(fcgi-http-svr
	content_type.cpp
	http_fcgi_svr.cpp
	http_fcgi_work_thread.cpp
	http_req_callback_base.cpp
	http_req_callback_file.cpp
	http_req_util.cpp
	http_util.cpp
	http_common.cpp
	http_req_error_handler.cpp
	thread_base.cpp
	http_req_jsonrpc.cpp

	path/Directory_tree.cpp
	path/Directory_tree_node.cpp
	path/Path_util.cpp

)

target_link_libraries(fcgi-http-svr
	PUBLIC

		Boost::boost
		Boost::filesystem
		spdlog::spdlog
		fcgi
		uriparser
		
		nlohmann_json::nlohmann_json
		json-rpc-cxx
)

target_include_directories(fcgi-http-svr
	PUBLIC
		include
)

target_compile_options(fcgi-http-svr
	PUBLIC
)

set_target_properties(fcgi-http-svr
PROPERTIES
	VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
	SOVERSION ${PROJECT_VERSION_MAJOR}
)

INSTALL(
	DIRECTORY
		include/
	DESTINATION
		include
)

INSTALL(
	TARGETS
		fcgi-http-svr
	DESTINATION
		lib
)

add_subdirectory(tests)
