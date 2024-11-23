set(LIB_NAME "cmlibScratchBuffer")

set(SOURCES "ScratchBuf.c")

add_library(${LIB_NAME}  ${SOURCES})

target_include_directories(${LIB_NAME} INTERFACE .)
target_link_libraries(${LIB_NAME} PUBLIC cmlibString)
