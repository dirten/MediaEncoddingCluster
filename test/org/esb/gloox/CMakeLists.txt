include_directories (${GLOOX_INCLUDE_DIR})


add_executable (GlooxConnectTest GlooxConnectTest.cpp)

target_link_libraries (GlooxConnectTest ${GLOOX_LIBRARY_DIR} )

IF(NOT WIN32)
	target_link_libraries (GlooxConnectTest resolv z ssl crypto)
ENDIF(NOT WIN32)
IF(UNIX AND NOT APPLE)
	target_link_libraries (GlooxConnectTest idn pthread)
ENDIF(UNIX AND NOT APPLE)
IF(UNIX)
    target_link_libraries (GlooxConnectTest resolv z ssl crypto)
ENDIF(UNIX)
