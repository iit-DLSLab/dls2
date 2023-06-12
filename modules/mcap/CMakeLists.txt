add_compile_options("-std=c++17")

add_library(dls_mcap
	SHARED
 		src/mcap_utils.cpp
)

target_include_directories(dls_mcap
	PUBLIC
		include
		include/third_party
)

target_link_libraries(dls_mcap
	PUBLIC
		lz4
		zstd
)

dls_install(dls_mcap)