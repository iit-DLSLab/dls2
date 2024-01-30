set (LIBRARY_NAME dls_action_client_stock)
add_library(${LIBRARY_NAME} SHARED
	src/action_client_stock.cpp
)
set_property(TARGET ${LIBRARY_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(${LIBRARY_NAME}
	PUBLIC
		include
)

target_link_libraries(${LIBRARY_NAME}
	PUBLIC
		robotlib
        goHome
		goFold
)
# target_link_directories(${LIBRARY_NAME}
# 	PUBLIC
# 		${DLS_INSTALL_ACTION_DIR}
# )
# ================================== Install =================================

dls_install(${LIBRARY_NAME})