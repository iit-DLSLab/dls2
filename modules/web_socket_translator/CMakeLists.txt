add_compile_options("-std=c++20")

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

install(
    TARGETS             dls_web_socket_translator
    LIBRARY DESTINATION ${DLS_INSTALL_SERVICE_DIR}
    COMPONENT           dls_runtime
)

install(
	DIRECTORY 		json/
	DESTINATION     /usr/lib/dls2/services/web_socket_translator
	COMPONENT		dls_runtime
	FILES_MATCHING
		PATTERN "*.json"
)