cmake_minimum_required(VERSION 3.30)
project(cmlib)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED true)

set(CMAKE_C_FLAGS "-O0 -fsanitize=address")

add_subdirectory(Logger)
add_subdirectory(ScratchBuffer)
add_subdirectory(Vector)
add_subdirectory(String)
add_subdirectory(IO)
add_subdirectory(Allocator)
add_subdirectory(List)
add_subdirectory(tests)

set(CMAKE_C_FLAGS_DEBUG "-fsanitize=address")
