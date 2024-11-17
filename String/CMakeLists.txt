set(SOURCES
    "String.c")

add_library(String ${SOURCES})

target_include_directories(String PUBLIC ../Error)
target_link_libraries(String Error)
