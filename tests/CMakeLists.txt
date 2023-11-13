add_executable(fcgi-http-lib-tests
	http_common_tests.cpp
)

target_link_libraries(fcgi-http-lib-tests
	fcgi-http-svr

	googletest_main
)

INSTALL(
	TARGETS
		fcgi-http-lib-tests
	DESTINATION
		tests
)
