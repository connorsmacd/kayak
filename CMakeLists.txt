cmake_minimum_required(VERSION 3.10)

project(kayak LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include(CTest)

add_subdirectory(external)

add_subdirectory(src)

if(BUILD_TESTING)
    add_subdirectory(test)
endif()
