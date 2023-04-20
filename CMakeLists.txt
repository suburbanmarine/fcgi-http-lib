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

)

target_link_libraries(fcgi-http-svr
	PUBLIC

		Boost::boost
		Boost::filesystem
		spdlog::spdlog
		fcgi
		
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

INSTALL(
	TARGETS
		fcgi-http-svr
	DESTINATION
		lib
)
