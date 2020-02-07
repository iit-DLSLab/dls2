add_library(
	dls_estimator
	SHARED
	estimator.cpp
)

# list(APPEND PROJECT_TARGETS dls_estimator)
dls_install(dls_estimator)
add_library(
	dls_dummy_estimator
	SHARED
	dummy_estimator.cpp
)
add_dependencies(dls_dummy_estimator messagelib doglib)
# list(APPEND PROJECT_TARGETS dls_dummy_estimator)
dls_install(dls_dummy_estimator)
