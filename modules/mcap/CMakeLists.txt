add_compile_options("-std=c++20")

# Common libraries/folders to be linked/included
set(LIBS lz4 zstd dls_com dls_msgs_wrappers dls_logging)
set(DIRS "include" "include/third_party")

# MCAP writer library
add_library(dls_mcap_writer SHARED src/mcap_writer_utils.cpp)
target_include_directories(dls_mcap_writer PUBLIC ${DIRS})
target_link_libraries(dls_mcap_writer PUBLIC ${LIBS})

# MCAP reader library
add_library(dls_mcap_reader SHARED src/mcap_reader_utils.cpp src/mcap_reader_support.cpp)
target_include_directories(dls_mcap_reader PUBLIC ${DIRS})
target_link_libraries(dls_mcap_reader PUBLIC ${LIBS})

# MCAP interface library
add_library(dls_mcap INTERFACE)
target_link_libraries(dls_mcap INTERFACE dls_mcap_writer dls_mcap_reader)

# Installation of MCAP libraries
dls_install(dls_mcap_writer)
dls_install(dls_mcap_reader)
dls_install(dls_mcap)