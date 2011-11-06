include_directories (${GLOOX_INCLUDE_DIR})


add_executable (GlooxConnectTest GlooxConnectTest.cpp)

target_link_libraries (GlooxConnectTest ${GLOOX_LIBRARY})

