set(LIB_NAME "cmlibVector")

set(SOURCES
    "src/Vector.c"
)

add_library(${LIB_NAME} STATIC ${SOURCES})

target_include_directories(${LIB_NAME} PUBLIC .)
target_link_libraries(${LIB_NAME} PUBLIC cmlibLogger)
