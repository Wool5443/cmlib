cmake_minimum_required(VERSION 3.30)
project(cmlib)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED true)

set(CMAKE_C_FLAGS_DEBUG "-fsanitize=address -O0 -g")

add_subdirectory(Allocator)
add_subdirectory(Hash_table)
add_subdirectory(IO)
add_subdirectory(List)
add_subdirectory(Logger)
add_subdirectory(Scratch_buffer)
add_subdirectory(String)
add_subdirectory(tests)
add_subdirectory(Vector)
