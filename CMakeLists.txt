cmake_minimum_required(VERSION 3.10)

project(kayak
    VERSION 0.0.0
    LANGUAGES CXX
    DESCRIPTION "A C++20 reflection library with minimal macro usage"
    HOMEPAGE_URL "https://https://github.com/connorsmacd/kayak"
)

set(CMAKE_CXX_STANDARD 20)

include(CTest)

add_subdirectory(external)

add_subdirectory(src)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

option(BUILD_EXAMPLES "Build examples" ON)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

