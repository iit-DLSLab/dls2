add_executable(topic_info
	EXCLUDE_FROM_ALL
		src/main.cpp
		src/topic_info.cpp
)

target_link_libraries(topic_info
	PRIVATE
		dls_com
		dls_debug
)

add_executable(dls_hz
	src/message_hz.cpp
	src/LookupListener.cpp

	src/utils/DataTypeConfiguration.hpp
	src/utils/dynamic_types_utils.cpp
	src/utils/utils.cpp

	src/Participant.cpp
	src/ReaderHandler.cpp
	# src/topic_info.cpp
)

target_link_libraries(dls_hz
	dls_framework
	dls_messages
	fastrtps
	fastcdr
)

dls_install(dls_hz)

