include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY  https://github.com/google/googletest.git
    GIT_TAG         v1.18.0
)

FetchContent_MakeAvailable(googletest)

include(GoogleTest)

# ------------------ TESTS -------------------
add_executable(rsa_lib_tests_exe 
    rsa_test.cpp
    square_and_multiply_test.cpp
    extended_euclidean_algorithm_test.cpp
    miller_rabin_test.cpp
    prime_generator_test.cpp
    rsa_decryptor_test.cpp
)

target_link_libraries(rsa_lib_tests_exe 
    PRIVATE
        gtest_main
        lib_rsa
)

gtest_discover_tests(rsa_lib_tests_exe
    PROPERTIES
        ENVIRONMENT "GTEST_COLOR=1"
)
