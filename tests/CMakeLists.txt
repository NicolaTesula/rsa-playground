include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY  https://github.com/google/googletest.git
    GIT_TAG         v1.18.0
)

FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED) # apparently unnecessary
target_link_libraries(GTest::GTest INTERFACE gtest_main)



# ------------------ TESTS -------------------

add_executable(rsa_test rsa_test.cpp)
add_executable(square_and_multiply_test square_and_multiply_test.cpp)
add_executable(extended_euclidean_algorithm_test extended_euclidean_algorithm_test.cpp)

target_link_libraries(rsa_test
    PRIVATE
        GTest::GTest
        lib_rsa
)
target_link_libraries(square_and_multiply_test
    PRIVATE 
        GTest::GTest
        lib_rsa
)
target_link_libraries(extended_euclidean_algorithm_test
    PRIVATE
        GTest::GTest
        lib_rsa
)

add_test(rsa_encryption_tests rsa_test)
add_test(square_and_multiply_tests square_and_multiply_test)
add_test(extended_euclidean_algorithm_tests extended_euclidean_algorithm_test)