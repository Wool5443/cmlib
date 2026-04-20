set(LIB_NAME cmlib_error)

set(SOURCES
    src/Error.c
)

add_library(${LIB_NAME} STATIC ${SOURCES})

target_include_directories(${LIB_NAME} PUBLIC .)
