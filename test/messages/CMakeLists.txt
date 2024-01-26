add_executable(wrapper_tests
		src/wrapper_tests.cpp
)

target_link_libraries(wrapper_tests
	PRIVATE
		dls_messages
		dls_framework
		robotlib
)
add_executable(signal_reader_writer_tests
		src/signal_reader_writer_tests.cpp
)

target_link_libraries(signal_reader_writer_tests
	PRIVATE
		dls_messages
		dls_framework
		robotlib
)
