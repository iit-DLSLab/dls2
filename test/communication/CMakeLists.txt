# find_package(fastcdr 2 REQUIRED)
# find_package(fastdds 3 REQUIRED)

add_executable(writer_communication_test    
    writer.cpp
)
target_link_libraries(writer_communication_test
    dls_com
    dls_messages
)