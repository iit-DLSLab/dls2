add_compile_options("-std=c++2a")

dls_configure(src/layer.cpp.in  LAYER_CPP)

add_library(dls_app
	SHARED
		src/sched_attr.cpp
		src/app_data.cpp
		src/app.cpp
		${LAYER_CPP}
		src/periodic_app.cpp
)

set_property(TARGET dls_app PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_app
	PUBLIC
		include
	PRIVATE
		src
)

target_link_libraries(dls_app
	PUBLIC
		yaml-cpp
		dls_com
		dls_time
		dls_command
		dls_logging
		dls_class_loader
		dls_topics
		dls_domains
)

dls_install(dls_app)