include_directories (${MEC_SOURCE_DIR}/src)

add_executable (GridNodeTest GridNodeTest.cpp)

target_link_libraries (GridNodeTest grid )

ADD_TEST(GridNodeTest ${EXECUTABLE_OUTPUT_PATH}/GridNodeTest)


