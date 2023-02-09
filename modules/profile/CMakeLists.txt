add_library(dls_profile
	src/profile.cpp
)

set_property(TARGET dls_profile PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_profile
	PUBLIC
		include
)

target_link_libraries(dls_profile
	PUBLIC
		yaml-cpp
)

dls_install(dls_profile)