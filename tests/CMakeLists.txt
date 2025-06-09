add_executable(test_enum_description enum_description.cpp)
target_link_libraries(test_enum_description PRIVATE kayak Catch2::Catch2WithMain)
add_test(NAME test_enum_description COMMAND test_enum_description)

add_executable(test_fixed_string fixed_string.cpp)
target_link_libraries(test_fixed_string PRIVATE kayak Catch2::Catch2WithMain)
add_test(NAME test_fixed_string COMMAND test_fixed_string)

add_executable(test_std std.cpp)
target_link_libraries(test_std PRIVATE kayak Catch2::Catch2WithMain)
add_test(NAME test_std COMMAND test_std)

add_executable(test_struct_description struct_description.cpp)
target_link_libraries(test_struct_description PRIVATE kayak Catch2::Catch2WithMain)
add_test(NAME test_struct_description COMMAND test_struct_description)

add_executable(test_struct_update struct_update.cpp)
target_link_libraries(test_struct_update PRIVATE kayak Catch2::Catch2WithMain)
add_test(NAME test_struct_update COMMAND test_struct_update)

add_executable(test_type_description type_description.cpp)
target_link_libraries(test_type_description PRIVATE kayak Catch2::Catch2WithMain)
add_test(NAME test_type_description COMMAND test_type_description)
