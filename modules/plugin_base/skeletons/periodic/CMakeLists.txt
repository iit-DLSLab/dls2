cmake_minimum_required(VERSION 3.15)   
project(${PLUGIN_NAME})

# Copy README.md
file(	COPY README.md # this is just a copy
		DESTINATION .
)
# Copy githooks
file(	COPY githooks # this is just a copy
		DESTINATION .
)
# Copy setup.sh
file(	COPY setup.sh # this is just a copy
		DESTINATION .
)
# Copy version.txt
file(	COPY version.txt # this is just a copy
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
file(	COPY plugin/CMakeLists.txt # this is just a copy
		DESTINATION plugin
)
# -- core
configure_file(plugin/core/CMakeLists.txt.in
	plugin/core/CMakeLists.txt @ONLY
)
configure_file(plugin/core/include/plugin.hpp.in
	plugin/core/include/${PLUGIN_TYPE}/${PLUGIN_NAME}/plugin.hpp @ONLY
)
configure_file(plugin/core/src/plugin.cpp.in
	plugin/core/src/plugin.cpp @ONLY
)
file(	COPY plugin/core/config/scheduler.yaml # this is just a copy
		DESTINATION plugin/core/config
)
# -- console_commands
configure_file(plugin/console_commands/CMakeLists.txt.in
	plugin/console_commands/CMakeLists.txt @ONLY
)
configure_file(plugin/console_commands/include/console_commands.hpp.in
	plugin/console_commands/include/${PLUGIN_TYPE}/${PLUGIN_NAME}/console_commands.hpp @ONLY
)
configure_file(plugin/console_commands/src/console_commands.cpp.in
	plugin/console_commands/src/console_commands.cpp @ONLY
)

# -- messages
configure_file(plugin/messages/CMakeLists.txt.in
	plugin/messages/CMakeLists.txt @ONLY
)
file(	COPY plugin/messages/idls/message.idl # this is just a copy
		DESTINATION plugin/messages/idls
)
file(	COPY plugin/messages/cmake/dls_message.cmake # this is just a copy
        DESTINATION plugin/messages/cmake
)
file(	COPY plugin/messages/include/message.hpp # this is just a copy
        DESTINATION plugin/messages/include/${PLUGIN_TYPE}/${PLUGIN_NAME}
)
file(	COPY plugin/messages/src/message.cpp # this is just a copy
        DESTINATION plugin/messages/src
)

# -- topics
configure_file(plugin/topics/CMakeLists.txt.in
	plugin/topics/CMakeLists.txt @ONLY
)
configure_file(plugin/topics/include/topics.hpp.in
	plugin/topics/include/${PLUGIN_TYPE}/${PLUGIN_NAME}/topics.hpp @ONLY
)
configure_file(plugin/topics/src/topics.cpp.in
	plugin/topics/src/topics.cpp @ONLY
)
