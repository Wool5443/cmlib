set(SOURCES
    "ScratchBuf.c")

add_library(ScratchBuf ${SOURCES})

target_include_directories(ScratchBuf PUBLIC ../Error ../String)
target_link_libraries(ScratchBuf Error String)
