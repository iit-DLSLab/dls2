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
