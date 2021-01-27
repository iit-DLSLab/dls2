set(GRAPHVIZ_EXTERNAL_LIBS FALSE)
set(GRAPHVIZ_UNKNOWN_LIBS  FALSE)
list(APPEND GRAPHVIZ_IGNORE_TARGETS
	"launch_framework_from_src_tree"
	"compile_test"
	"dummy" # dummy controllers, gait generators etc
)
