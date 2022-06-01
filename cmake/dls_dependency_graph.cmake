message(STATUS "## Reading dls_dependency_graph.cmake")

add_custom_target(graph
	COMMAND           ${CMAKE_COMMAND} "--graphviz=dependency_graph.dot" ${CMAKE_SOURCE_DIR}
	COMMAND dot -Tpng dependency_graph.dot                                  -o dependency_graph_full.png
	COMMAND dot -Tpng dependency_graph.dot.dynamic_legged_systems_framework -o dependency_graph_framework.png
	COMMAND dot -Tpng dependency_graph.dot.topic_info                       -o dependency_graph_topic_info.png
	COMMAND dot -Tpng dependency_graph.dot.dls_parameter_server_cli         -o dependency_graph_parameter_server.png
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
