add_library(dls_estimator
	SHARED
		src/estimator.cpp
)

target_include_directories(dls_estimator
	PUBLIC
		include
)

target_link_libraries(dls_estimator
	PUBLIC
		robotlib
		dls_framework_components
)

dls_install(dls_estimator)
