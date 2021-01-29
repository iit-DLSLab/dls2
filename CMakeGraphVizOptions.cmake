set(GRAPHVIZ_EXTERNAL_LIBS  FALSE)
set(GRAPHVIZ_UNKNOWN_LIBS   FALSE)
# set(GRAPHVIZ_INTERFACE_LIBS FALSE)
list(APPEND GRAPHVIZ_IGNORE_TARGETS
	"launch_framework_from_src_tree"
	# "child_process_launcher"
	"compile_test"
	"dls_dls2_messaging"
	"dummy" # dummy controllers, gait generators etc
)
