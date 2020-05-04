add_library(
	dls_geometry
	SHARED
	pose.cpp
)

add_dependencies(dls_geometry dls_messaging)
dls_install(dls_geometry)
add_dependencies(dls_geometry dls_dls2_messaging)
