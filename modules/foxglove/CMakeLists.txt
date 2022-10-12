# add_library(dls_foxglove
# 	src/server.cpp
# )

add_executable(dls_foxglove
src/server.cpp
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
)

dls_install(dls_foxglove)
