add_compile_options("-std=c++17")

add_library(dls_foxglove
	SHARED
 		src/webserver.cpp
		src/foxserver.cpp
		src/dynamic_types_utils.cpp
		src/utils.cpp
)

target_include_directories(dls_foxglove
	PUBLIC
		include
)

target_link_libraries(dls_foxglove
	PRIVATE
        boost_system
        pthread
		dls_app
		dls_com
		dls_topics
		dls_domains
		fastrtps
)

dls_install(dls_foxglove)

install(
	DIRECTORY 	json/
	DESTINATION	/usr/lib/dls2/dls_foxglove
	COMPONENT	dls_runtime
	FILES_MATCHING
		PATTERN "*.json"
)