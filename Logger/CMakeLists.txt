set(LIB_NAME "cmlibLogger")

set(SOURCES "src/Logger.c")

add_library(${LIB_NAME} STATIC ${SOURCES})

target_include_directories(${LIB_NAME} INTERFACE .)
