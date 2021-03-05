add_library(dls_geometry
	src/pose.cpp
)

set_property(TARGET dls_geometry PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_geometry
	PUBLIC
		include
)

target_link_libraries(dls_geometry
	PUBLIC
		dls_dls2_messaging
)

dls_install(dls_geometry)
