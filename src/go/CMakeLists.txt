
add_subdirectory (pkg)

add_go_executable (mainrpc mainrpc.go main.go)
TARGET_LINK_GO_LIBRARIES(mainrpc
    hivenet
)



