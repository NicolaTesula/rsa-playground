add_library(lib_rsa rsa.cpp square_and_multiply.cpp)

target_include_directories(lib_rsa PUBLIC include)

target_compile_definitions(lib_rsa PRIVATE DEBUG=1)

# Need to link clang internal function on windows. TODO: create own function to circumvent this
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} --print-resource-dir
        OUTPUT_VARIABLE CLANG_RESOURCE_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    target_link_libraries(lib_rsa PUBLIC "${CLANG_RESOURCE_DIR}/lib/windows/clang_rt.builtins-x86_64.lib")
endif()