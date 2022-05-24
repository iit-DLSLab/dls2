add_library(dls_command SHARED
	src/command_base.cpp
	src/command.cpp
	src/command_manager.cpp
)

target_include_directories(dls_command
	PUBLIC
		include
)

target_link_libraries(dls_command
	PUBLIC
		dls_pubsub
		dls_topics
		dls_domains
)

dls_install(dls_command)

target_compile_options(dls_command
	PRIVATE
		-fPIC
)

add_subdirectory(test)
