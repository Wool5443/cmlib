set(LIB_NAME cmlib_list)

set(SOURCES
    src/List.c
)

add_library(${LIB_NAME} STATIC ${SOURCES})

target_include_directories(${LIB_NAME} PUBLIC .)
target_link_libraries(${LIB_NAME} PUBLIC cmlib_logger cmlib_allocator)
