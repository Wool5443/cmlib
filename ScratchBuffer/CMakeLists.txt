set(LIB_NAME cmlib_scratch_buffer)

set(SOURCES src/ScratchBuf.c)

add_library(${LIB_NAME} STATIC ${SOURCES})

target_include_directories(${LIB_NAME} PUBLIC .)
target_link_libraries(${LIB_NAME} PUBLIC cmlib_string)
