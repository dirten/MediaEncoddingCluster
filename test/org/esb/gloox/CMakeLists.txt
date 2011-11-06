include_directories (${GLOOX_INCLUDE_DIR})


add_executable (GlooxConnectTest GlooxConnectTest.cpp)

target_link_libraries (GlooxConnectTest ${GLOOX_LIBRARY_DIR} resolv z ssl crypto)

IF(NOT WIN32)
	target_link_libraries (GlooxConnectTest resolv z ssl crypto)
ENDIF(NOT WIN32)
IF(LINUX)
	target_link_libraries (GlooxConnectTest idn pthread)
ENDIF(LINUX)

