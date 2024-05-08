add_subdirectory(third_party/foxglove-websocket)

add_library(dls_data_visualizer
	SHARED
		src/data_visualizer_base.cpp)

set_property(TARGET dls_data_visualizer PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_data_visualizer
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/include
		${CMAKE_CURRENT_SOURCE_DIR}/third_party/foxglove-websocket/include
)

target_link_libraries(dls_data_visualizer
	PUBLIC
		foxglove_websocket
		robotlib
		dls_app
		dls_message_wrappers
)

dls_install(dls_data_visualizer)

install(
    DIRECTORY       ${CMAKE_CURRENT_SOURCE_DIR}/json/
	DESTINATION     ${DLS_INSTALL_DATA_VISUALIZER_DIR}/json
	COMPONENT       dls_runtime
	FILES_MATCHING
		PATTERN "*.json"
)