set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")

add_executable(
	dls2
	main.cpp
	application_framework/hyq_app.cpp
	application_framework/app_layer.cpp
)

target_link_libraries(
	dls2
)
