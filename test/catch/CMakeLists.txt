add_library(catch2 SHARED
	src/catch2.cpp
)

target_include_directories(catch2
	PUBLIC
		include
)

set_target_properties(catch2 PROPERTIES EXCLUDE_FROM_ALL 1)

add_dependencies(dls2-tests catch2)
