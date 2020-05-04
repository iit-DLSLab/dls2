add_library(
	dls_estimator
	SHARED
	estimator.cpp
)
dls_install(dls_estimator)
add_dependencies(dls_estimator dls_dls2_messaging)

add_library(
	dls_dummy_estimator
	SHARED
	dummy_estimator.cpp
)
# add_dependencies(dls_dummy_estimator messagelib doglib)
dls_install_debug(dls_dummy_estimator)
add_dependencies(dls_dummy_estimator dls_dls2_messaging)
