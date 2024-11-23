set(LIB_NAME "cmlibString")

set(SOURCES "String.c")

add_library(${LIB_NAME} ${SOURCES})

target_include_directories(${LIB_NAME} INTERFACE .)
target_link_libraries(${LIB_NAME} PUBLIC cmlibError)
