add_library(dls_math INTERFACE)
target_include_directories(dls_math INTERFACE include)
target_link_libraries(dls_math INTERFACE Eigen3::Eigen)

dls_install(dls_math)
