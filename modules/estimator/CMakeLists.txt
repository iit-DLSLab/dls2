add_library(dls_estimator
	SHARED
		src/estimator.cpp
)

target_include_directories(dls_estimator
	PUBLIC
		include
)

target_link_libraries(dls_estimator
	PRIVATE
		robotlib
		dls_app
		dls_command
		dls_com
		dls_topics
		dls_domains
		dls_logging
)

dls_install(dls_estimator)
