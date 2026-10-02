include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY  https://github.com/google/googletest.git
    GIT_TAG         v1.18.0
)

FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED) # apparently unnecessary
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(rsa_test rsa_test.cpp)

target_link_libraries(rsa_test
    PRIVATE
        GTest::GTest
        lib_rsa
)


# -------- TESTS ---------

add_test(rsa_encrypt_test rsa_test)