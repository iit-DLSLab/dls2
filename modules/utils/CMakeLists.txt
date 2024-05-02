add_library(dls_utils
	src/utils.cpp
)

set_property(TARGET dls_utils PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_utils
	PUBLIC
		include
)

dls_install(dls_utils)