set(LIB_NAME "cmlibScratchBuffer")

set(SOURCES "src/ScratchBuf.c")

add_library(${LIB_NAME} STATIC ${SOURCES})

target_include_directories(${LIB_NAME} PUBLIC .)
target_link_libraries(${LIB_NAME} PUBLIC cmlibString)
