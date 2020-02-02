cmake_minimum_required (VERSION 3.10)
project (restarter CXX)
option (COVERAGE_ENABLED "Capture coverage data")

configure_file (
    config.hpp.in
    "${CMAKE_CURRENT_BINARY_DIR}/config.hpp"
    ESCAPE_QUOTES
)

add_executable (restarter
    main.cpp
    restarter.cpp
    restarter.hpp
    trace.hpp
)
set_target_properties (restarter PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED Yes
    CXX_EXTENSIONS No
)
target_include_directories (restarter PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}"
)
find_package (Threads)
target_link_libraries (restarter ${CMAKE_THREAD_LIBS_INIT})
