add_executable(bin_playground main.cpp
    tui.cpp)

target_include_directories(bin_playground PUBLIC include)

target_link_libraries(bin_playground PRIVATE lib_rsa)

set_target_properties(bin_playground PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)