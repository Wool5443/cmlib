set(LIB_NAME cmlib_allocator)

set(SOURCES
    src/Allocator.c
    src/Arena.c
    src/ArenaResource.c
    src/FreeList.c
    src/FreeListResource.c
    src/CountingMalloc.c
    src/Pool.c
    src/PoolResource.c
)

add_library(${LIB_NAME} STATIC ${SOURCES})

target_include_directories(${LIB_NAME} PUBLIC .)
target_link_libraries(${LIB_NAME} PUBLIC cmlib_error)
