add_compile_options("-std=c++17")

dls_configure(src/app_layer.cpp.in  APP_LAYER_CPP)

add_library(dls_core
	SHARED
		src/app.cpp
		${APP_LAYER_CPP}
)

set_property(TARGET dls_core PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_core
	PUBLIC
		include
	PRIVATE
		src
)

target_link_libraries(dls_core
	PUBLIC
		dls_com
		dls_command
		dls_logging
		dls_framework_components
		dls_class_loader
)

dls_install(dls_core)