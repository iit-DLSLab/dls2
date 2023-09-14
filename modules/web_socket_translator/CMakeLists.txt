add_compile_options("-std=c++17")

add_library(dls_web_socket_translator
	SHARED
		src/web_socket_translator.cpp
		src/webserver.cpp
		src/dynamic_types_utils.cpp
		src/utils.cpp
)

set_property(TARGET dls_web_socket_translator PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_web_socket_translator
	PUBLIC
		include
)

target_link_libraries(dls_web_socket_translator
	PUBLIC
		boost_system
		dls_framework
)

dls_install(dls_web_socket_translator)

install(
	DIRECTORY 	json/
	DESTINATION	/usr/lib/dls2/web_socket_translator
	COMPONENT	dls_runtime
	FILES_MATCHING
		PATTERN "*.json"
)