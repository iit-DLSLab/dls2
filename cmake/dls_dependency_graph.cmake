add_custom_target(graph
	COMMAND           ${CMAKE_COMMAND} "--graphviz=dependency_graph.dot" ${CMAKE_SOURCE_DIR}
	COMMAND           dot -Tpng dependency_graph.dot -o dependency_graph.png
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
