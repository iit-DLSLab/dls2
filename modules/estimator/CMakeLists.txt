add_library(dls_estimator
	src/estimator.cpp
)

target_include_directories(dls_estimator
	PUBLIC
		include
)

target_link_libraries(dls_estimator
	PUBLIC
		dls_framework_components
)

add_dependencies(dls_estimator
	dls_dls2_messaging
)

dls_install(dls_estimator)
