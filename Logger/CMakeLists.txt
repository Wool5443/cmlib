set(LIB_NAME cmlib_logger)

set(SOURCES
    src/Logger.c
    src/Error.c
    src/Console_color.c
)

add_library(${LIB_NAME} STATIC ${SOURCES})

target_include_directories(${LIB_NAME} PUBLIC .)
