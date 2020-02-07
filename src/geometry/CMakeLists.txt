add_library(
	dls_geometry
	SHARED
	pose.cpp
)

add_dependencies(dls_geometry dls_messaging)
# list(APPEND PROJECT_TARGETS dls_geometry)
dls_install(dls_geometry)
