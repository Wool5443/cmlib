set(LIB_NAME "cmlibError")

set(SOURCES "Error.c")

add_library(${LIB_NAME} ${SOURCES})

target_include_directories(${LIB_NAME} INTERFACE .)
