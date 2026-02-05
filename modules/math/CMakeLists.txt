add_library(dls_math
    src/frenet_path.cpp
)

target_include_directories(dls_math PUBLIC include)

add_subdirectory(test)

dls_install(dls_math)