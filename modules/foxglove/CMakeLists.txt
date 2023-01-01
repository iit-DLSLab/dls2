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
	PUBLIC
        boost_system
        pthread
		dls_framework_components
		fastrtps
)

dls_install(dls_foxglove)
