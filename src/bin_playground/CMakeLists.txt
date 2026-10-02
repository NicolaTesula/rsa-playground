add_executable(bin_playground main.cpp)

target_include_directories(bin_playground PUBLIC include)

set_target_properties(bin_playground PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)