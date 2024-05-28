add_executable(realtime_test
		src/realtime_test.cpp
)

target_link_libraries(realtime_test
	PRIVATE
		dls_app
)

dls_install(realtime_test)