add_library(dls_signal 
	SHARED
		src/signal_reader_base.cpp
		src/signal_writer_base.cpp
		src/signal_base.cpp
		src/reader_base.cpp
		src/writer_base.cpp
)

set_property(TARGET dls_signal PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_signal
	PUBLIC
		include
)

target_link_libraries(dls_signal
	PUBLIC
		Eigen3::Eigen
		dls_com
		dls_topics
		dls_time		
)

add_subdirectory(test)

dls_install(dls_signal)