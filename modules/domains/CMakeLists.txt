add_library(dls_domains SHARED
		src/domains.cpp
)

target_include_directories(dls_domains
	PUBLIC
		include
)

dls_install(dls_domains)
