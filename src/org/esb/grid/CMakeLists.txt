


include_directories (${MEC_SOURCE_DIR}/target/dependency/include)

add_library (grid
  GridRegistry.cpp
  GridNode.cpp
)

TARGET_LINK_LIBRARIES(grid

)

IF(BUILD_SHARED_LIBS)
INSTALL(TARGETS grid
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
ENDIF(BUILD_SHARED_LIBS)
