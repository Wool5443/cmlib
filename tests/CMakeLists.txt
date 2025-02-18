add_executable(testLog Log.c)
target_link_libraries(testLog PRIVATE  cmlib_logger)

add_executable(testVec Vec.c)
target_link_libraries(testVec PRIVATE cmlib_vector)

