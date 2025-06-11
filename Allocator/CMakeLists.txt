set(LIB_NAME cmlib_allocator)

set(SOURCES
    src/Allocator.c
    src/Arena.c
    src/Free_list.c
)

add_library(${LIB_NAME} STATIC ${SOURCES})

target_include_directories(${LIB_NAME} PUBLIC .)
target_link_libraries(${LIB_NAME} PUBLIC cmlib_logger)
