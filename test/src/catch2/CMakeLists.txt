add_library(catch2
	catch2.cpp
)
set_target_properties(catch2 PROPERTIES EXCLUDE_FROM_ALL 1)
add_dependencies("tests" catch2)
