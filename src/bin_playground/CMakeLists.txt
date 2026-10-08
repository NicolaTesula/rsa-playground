include(FetchContent)
 
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v7.0.3  # Replace with a version, tag, or commit hash
)

# Switch these to customize what to build from FTXUI
set(FTXUI_BUILD_EXAMPLES ON CACHE BOOL "" FORCE)
set(FTXUI_BUILD_DOCS     OFF CACHE BOOL "" FORCE) # Need to have doxygen installed
set(FTXUI_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
 
FetchContent_MakeAvailable(ftxui)

add_executable(bin_playground 
    main.cpp
    tui.cpp
    views/view_main_menu.cpp
    views/view_generate_primes.cpp
)

target_include_directories(bin_playground PUBLIC include)

target_link_libraries(bin_playground 
    PRIVATE lib_rsa
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
)

set_target_properties(bin_playground PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)