add_executable(test_log Log.c)
target_link_libraries(test_log PRIVATE cmlib_logger)

add_executable(test_vec Vec.c)
target_link_libraries(test_vec PRIVATE cmlib_vector)

add_executable(test_arena_allocator Arena_allocator.c)
target_link_libraries(test_arena_allocator PRIVATE cmlib_vector)

add_executable(test_free_list_allocator Free_list_allocator.c)
target_link_libraries(test_free_list_allocator PRIVATE cmlib_vector)

add_executable(test_string String.c)
target_link_libraries(test_string PRIVATE cmlib_string)

add_executable(test_list List.c)
target_link_libraries(test_list PRIVATE cmlib_list)
