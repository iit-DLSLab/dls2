cmake_minimum_required(VERSION 3.15)
project(FoxgloveWebSocket CXX)

find_package(nlohmann_json REQUIRED)
find_package(websocketpp REQUIRED)
find_package(OpenSSL REQUIRED)

add_library(foxglove_websocket SHARED src/base64.cpp src/parameter.cpp src/serialization.cpp src/server_factory.cpp)
target_include_directories(foxglove_websocket PUBLIC include ${OPENSSL_INCLUDE_DIR})
target_link_libraries(foxglove_websocket PUBLIC nlohmann_json::nlohmann_json websocketpp::websocketpp ${OPENSSL_LIBRARIES} -lz)
set_target_properties(foxglove_websocket PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

if (MSVC)
  target_compile_options(foxglove_websocket PRIVATE /WX /W4)
else()
  target_compile_options(foxglove_websocket PRIVATE -Wall -Wextra -Wpedantic -Werror -Wold-style-cast -Wfloat-equal)
endif()

dls_install(foxglove_websocket)

install(
	DIRECTORY   ${CMAKE_CURRENT_SOURCE_DIR}/include/
	COMPONENT   foxglove_websocket_dev
	DESTINATION ${DLS_INSTALL_DATA_VISUALIZER_HEADER_DIR}/foxglove_websocket
)
install(
  FILES ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE
  DESTINATION ${DLS_INSTALL_DATA_VISUALIZER_HEADER_DIR}/foxglove_websocket/licenses/
)