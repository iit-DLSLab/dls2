add_library(dls_service 
	SHARED
		src/service.cpp
)

set_property(TARGET dls_service PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_service
	PUBLIC
		include
)

target_link_libraries(dls_service
	PUBLIC
		dls_com
		dls_framework_components
)

dls_install(dls_service)
