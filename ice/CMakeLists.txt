
include_directories (${ICE_DIR}/include)
include_directories (${MEC_SOURCE_DIR}/ice)
include_directories (${MEC_SOURCE_DIR}/src)
LINK_DIRECTORIES(${ICE_DIR}/lib)

SET(ice_test_source
    TestUnit.cpp
    TestUnitImpl.cpp
    Server.cpp
    Client.cpp
)


add_executable (server   ${ice_test_source})

TARGET_LINK_LIBRARIES(server iced iceutild)
