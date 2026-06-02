add_executable(arena Arena.c)
target_link_libraries(
    arena
    PRIVATE cmlib_allocator cmlib_vector
)
add_executable(freelist FreeList.c)
target_link_libraries(
    freelist
    PRIVATE cmlib_allocator
)
add_executable(pool Pool.c)
target_link_libraries(
    pool
    PRIVATE cmlib_allocator cmlib_list
)
add_executable(io IO.c)
target_link_libraries(
    io
    PRIVATE cmlib_IO
)
add_executable(list List.c)
target_link_libraries(
    list
    PRIVATE cmlib_list
)
add_executable(log Log.c)
target_link_libraries(
    log
    PRIVATE cmlib_logger
)
add_executable(scratch Scratch.c)
target_link_libraries(
    scratch
    PRIVATE cmlib_scratch_buffer
)
add_executable(string String.c)
target_link_libraries(
    string
    PRIVATE cmlib_string
)
add_executable(vec Vec.c)
target_link_libraries(
    vec
    PRIVATE cmlib_vector
)
