if(BUILD_TESTING)
    find_package(Catch2 3 QUIET)

    if(NOT Catch2_FOUND)
        add_subdirectory(Catch2)
    endif()
endif()
