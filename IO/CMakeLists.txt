set(LIB_NAME cmlib_IO)

set(SOURCES
    src/IO.c
)

add_library(${LIB_NAME} STATIC ${SOURCES})

target_include_directories(${LIB_NAME} PUBLIC  .)
target_link_libraries(${LIB_NAME} PUBLIC cmlib_string)
