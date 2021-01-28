add_library(dls_geometry
	SHARED src/pose.cpp
)
target_include_directories(dls_geometry
	PUBLIC include
)
target_link_libraries(dls_geometry PUBLIC dls_dls2_messaging)

dls_install(dls_geometry)
