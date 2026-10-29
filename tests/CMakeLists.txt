include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY  https://github.com/google/googletest.git
    GIT_TAG         v1.18.0
    GIT_SHALLOW     TRUE
)

FetchContent_MakeAvailable(googletest)

include(GoogleTest)

# ------------------ TESTS -------------------
add_executable(rsa_lib_tests_exe
    square_and_multiply_test.cpp
    extended_euclidean_algorithm_test.cpp
    miller_rabin_test_test.cpp
    prime_generator_test.cpp
    block_encoding_test.cpp
    rsa_encryptor_test.cpp
    rsa_decryptor_test.cpp
    pollards_algorithm_test.cpp
    fermat_factorization_test.cpp
)

target_link_libraries(rsa_lib_tests_exe 
    PRIVATE
        gtest_main
        lib_rsa
)

target_compile_definitions(rsa_lib_tests_exe PRIVATE DEBUG=ON)

gtest_discover_tests(rsa_lib_tests_exe
    PROPERTIES
        ENVIRONMENT "GTEST_COLOR=1"
)
