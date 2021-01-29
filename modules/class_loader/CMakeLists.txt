add_library(dls_class_loader INTERFACE)
target_include_directories(dls_class_loader INTERFACE include)
target_link_libraries(dls_class_loader
	INTERFACE
		dls_logging
)
