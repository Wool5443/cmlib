set(LIB_NAME "cmlibIO")

add_library(${LIB_NAME} INTERFACE ${SOURCES})

target_include_directories(${LIB_NAME} INTERFACE  .)
target_link_libraries(${LIB_NAME} INTERFACE cmlibString cmlibLogger)
