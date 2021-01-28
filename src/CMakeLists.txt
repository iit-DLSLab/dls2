include(dls_include_paths)

# ==============================================================================
# Global Compilation Options
# ==============================================================================
add_subdirectory(application_framework)
add_subdirectory(child_process)
add_subdirectory(command)
add_subdirectory(controller)
add_subdirectory(estimator)
add_subdirectory(gait_generator)
add_subdirectory(geometry)
add_subdirectory(util)

# ==============================================================================
# Debugger
# ==============================================================================
add_custom_target(debug
	COMMAND           gdb ${EXECUTABLE_OUTPUT_PATH}/dynamic_legged_systems_framework
	DEPENDS           dynamic_legged_systems_framework
	WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
	COMMENT           "Launching debugger"
)

# ==============================================================================
# Header Export
# ==============================================================================
set(EXPORT_HEADERS "")
macro(export_header header)
	list(APPEND EXPORT_HEADERS header)
endmacro()

# ==============================================================================
# Libraries
# ==============================================================================
# ============================ Controller Modules ==============================
add_library(dls_reflex
	SHARED
	${CMAKE_CURRENT_SOURCE_DIR}/reflex/reflex
)

# ==============================================================================
# Convencience Targets
# ==============================================================================
get_property(LAUNCH_FRAMEOWRK_FROM_SRC_TREE
	TARGET   launch_framework_from_src_tree
	PROPERTY INTERFACE_SOURCES
)
