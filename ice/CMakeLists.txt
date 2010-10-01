
include_directories (${ICE_DIR}/include)
include_directories (${MEC_SOURCE_DIR}/ice)
include_directories (${MEC_SOURCE_DIR}/src)
LINK_DIRECTORIES(${ICE_DIR}/lib)



add_executable (server
    Printer.cpp
    Server.cpp
)
add_executable (client
    Printer.cpp
    Client.cpp
)

add_executable (gridserver
    Printer.cpp
    GridPrinterServer.cpp
)

TARGET_LINK_LIBRARIES(server Ice IceUtil)
TARGET_LINK_LIBRARIES(gridserver Ice IceUtil)
TARGET_LINK_LIBRARIES(client Ice IceUtil)
