cmake_minimum_required(VERSION 3.15)   
project(${PLUGIN_NAME})

# Copy version.txt
file(	COPY version.txt # this is just a copy
		DESTINATION .
)
# Copy .gitignore
file(	COPY .gitignore # this is just a copy
		DESTINATION .
)
# Configure outermost CMakeLists.txt
configure_file(CMakeLists.txt.in
	CMakeLists.txt @ONLY
)

# Configure module
configure_file(module/CMakeLists.txt.in
	module/CMakeLists.txt @ONLY
)
configure_file(module/include/module.hpp.in
	module/include/${PLUGIN_TYPE}/${PLUGIN_NAME}/${PLUGIN_NAME}.hpp @ONLY
)
configure_file(module/src/module.cpp.in
	module/src/${PLUGIN_NAME}.cpp @ONLY
)
file(	COPY module/config/config.yaml # this is just a copy
		DESTINATION module/config
)

# Configure plugin
configure_file(plugin/CMakeLists.txt.in
	plugin/CMakeLists.txt @ONLY
)
configure_file(plugin/include/plugin.hpp.in
	plugin/include/${PLUGIN_TYPE}/${PLUGIN_NAME}/plugin.hpp @ONLY
)
configure_file(plugin/src/plugin.cpp.in
	plugin/src/plugin.cpp @ONLY
)
file(	COPY plugin/config/scheduler.yaml # this is just a copy
		DESTINATION plugin/config
)
file(	COPY plugin/idls/message.idl # this is just a copy
		DESTINATION plugin/idls
)