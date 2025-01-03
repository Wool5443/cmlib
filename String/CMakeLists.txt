set(LIB_NAME "cmlibString")

set(SOURCES
    "src/String.c"
)

add_library(${LIB_NAME} STATIC ${SOURCES})

target_include_directories(${LIB_NAME} PUBLIC .)
target_link_libraries(${LIB_NAME} PUBLIC cmlibLogger)
