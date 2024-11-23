set(LIB_NAME "cmlibLogger")

set(SOURCES "implementation/Logger.c")

add_library(${LIB_NAME} ${SOURCES})

target_include_directories(${LIB_NAME} INTERFACE .)
