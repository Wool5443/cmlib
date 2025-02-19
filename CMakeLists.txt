cmake_minimum_required(VERSION 3.30)
project(cmlib)

add_subdirectory(Logger)
add_subdirectory(ScratchBuffer)
add_subdirectory(Vector)
add_subdirectory(String)
add_subdirectory(IO)
add_subdirectory(Allocator)
add_subdirectory(tests)

set(CMAKE_C_FLAGS_DEBUG "-fsanitize=address")
