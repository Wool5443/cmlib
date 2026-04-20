add_executable(cmlib_tests Tests.c)

target_link_libraries(
    cmlib_tests
    PRIVATE cmlib_allocator
            cmlib_IO
            cmlib_list
            cmlib_scratch_buffer
            cmlib_string
            cmlib_vector
)

add_test(NAME cmlib_tests COMMAND cmlib_tests)
