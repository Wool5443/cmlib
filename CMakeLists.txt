cmake_minimum_required(VERSION 3.30)
project(cmlib)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED true)

add_subdirectory(Logger)
add_subdirectory(Scratch_buffer)
add_subdirectory(Vector)
add_subdirectory(String)
add_subdirectory(IO)
add_subdirectory(Allocator)
add_subdirectory(List)
add_subdirectory(tests)
