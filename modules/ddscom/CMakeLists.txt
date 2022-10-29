add_library(dls_com SHARED
		src/dds_participant.cpp
		src/dds_listeners.cpp
		src/dds_reader.cpp
		src/dds_writer.cpp
)

target_include_directories(dls_com
	PUBLIC
		include
)

target_link_libraries(dls_com
	PRIVATE
		fastcdr
		fastrtps
		dls_domains
		dls_topics
)

set_property(TARGET dls_com PROPERTY POSITION_INDEPENDENT_CODE TRUE)

add_subdirectory(test)

dls_install(dls_com)