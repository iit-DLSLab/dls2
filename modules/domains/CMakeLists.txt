add_library(dls_domains INTERFACE)

target_include_directories(dls_domains
	INTERFACE
		include
)

dls_install(dls_domains)
