add_library(dls_time SHARED
	src/time.cpp
)
add_dependencies(dls_time dls_dls2_messaging)
dls_install(dls_time)
