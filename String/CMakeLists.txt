set(LIB_NAME cmlib_string)

set(SOURCES
    src/String.c
)

add_library(${LIB_NAME} STATIC ${SOURCES})

target_include_directories(${LIB_NAME} PUBLIC .)
target_link_libraries(${LIB_NAME} PUBLIC cmlib_logger cmlib_allocator)
