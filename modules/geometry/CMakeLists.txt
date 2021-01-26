add_library(dls_geometry
	SHARED src/pose.cpp
)
target_include_directories(dls_geometry
	PUBLIC include
)

dls_install(dls_geometry)
