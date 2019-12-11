create_test(
	multiple_publishers_as_fields
	${CMAKE_CURRENT_SOURCE_DIR}/multiple_publishers_as_fields.cpp
	"fastrtps;fastcdr;dls_messaging"
)

create_test(
	multiple_publishers_not_as_fields
	${CMAKE_CURRENT_SOURCE_DIR}/multiple_publishers_not_as_fields.cpp
	"fastrtps;fastcdr;dls_messaging"
)

create_test(
	fork_multiple_publishers_not_as_fields
	${CMAKE_CURRENT_SOURCE_DIR}/fork_multiple_publishers_not_as_fields.cpp
	"fastrtps;fastcdr;dls_messaging"
)

create_test(
	string_multiple_publishers_not_as_fields
	${CMAKE_CURRENT_SOURCE_DIR}/string_multiple_publishers_not_as_fields.cpp
	"fastrtps;fastcdr;dls_messaging;dls_logging"
)

create_test(
	controller_multiple_publishers_as_fields
	${CMAKE_CURRENT_SOURCE_DIR}/controller_multiple_publishers_as_fields.cpp
	"fastrtps;fastcdr;dls_messaging;dls_logging;dls_controlinterface;dls_framework;dls_gaitgeneratorinterface"
)

create_test(
	controller_multiple_publishers_not_as_fields
	${CMAKE_CURRENT_SOURCE_DIR}/controller_multiple_publishers_not_as_fields.cpp
	"fastrtps;fastcdr;dls_messaging;dls_logging;dls_controlinterface;dls_framework;dls_gaitgeneratorinterface"
)
