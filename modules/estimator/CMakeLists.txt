# ==============================================================================
# Estimator
# ==============================================================================
add_library(dls_estimator SHARED
	src/estimator.cpp
)
target_include_directories(dls_estimator
	PUBLIC
		include
)
add_dependencies(dls_estimator dls_dls2_messaging)
dls_install(dls_estimator)

# ==============================================================================
# Dummy Estimator for Testing
# ==============================================================================
add_library(dls_dummy_estimator SHARED
	src/dummy_estimator.cpp
)
target_include_directories(dls_dummy_estimator
	PUBLIC
		include
)
add_dependencies(dls_dummy_estimator dls_dls2_messaging)
dls_install_debug(dls_dummy_estimator)
