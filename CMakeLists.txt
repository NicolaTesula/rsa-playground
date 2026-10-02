# Created with inspiration from "Coding with Mat"

cmake_minimum_required(VERSION 4.4)
project(RSA_Playground LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # probably unnecessary

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)