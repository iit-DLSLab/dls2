set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")

add_compile_options(
	-std=c++17
	#	-Wfatal-errors
	-Wall
	-Wextra
	-pedantic
	-Weffc++
	-Werror
)

add_executable(
	dls2

	# Entry point
	main.cpp

	# Framework Classes
	application_framework/hyq_app.cpp
	application_framework/app_layer.cpp
	application_framework/hardware_layer.cpp
	application_framework/hyq_app_libraries.cpp
	application_framework/program_layer.cpp

	# Components
	application_framework/components/app_layer_component.cpp

	# Sensor Classes
	sensors/sensor_base.cpp

	# Controller Interface
	controller/controller.cpp
)

target_link_libraries(
	dls2
)
