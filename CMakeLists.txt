project(fcgi-http-svr
	VERSION 0.1.0
	DESCRIPTION "FastCGI utility library for C++"
	LANGUAGES C CXX
)

configure_file(./src/version.hpp.in version.hpp)

add_library(fcgi-http-svr
	src/content_type.cpp
	src/http_fcgi_svr.cpp
	src/http_fcgi_work_thread.cpp
	src/http_req_callback_base.cpp
	src/http_req_callback_file.cpp
	src/http_req_util.cpp
	src/http_util.cpp
	src/http_common.cpp
	src/http_req_error_handler.cpp
	src/thread_base.cpp
	src/http_req_jsonrpc.cpp

	src/path/Directory_tree.cpp
	src/path/Directory_tree_node.cpp
	src/path/Path_util.cpp

	src/version.cpp

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
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}
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
