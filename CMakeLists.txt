cmake_minimum_required(VERSION 3.15)

project(RSA_Playground LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # probably unnecessary

# Make dependency downloads visible and avoid silent "hanging"
set(FETCHCONTENT_QUIET OFF CACHE BOOL "" FORCE)

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)