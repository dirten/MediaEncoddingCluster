


include_directories (${MEC_SOURCE_DIR}/target/dependency/include/safmq)
add_library (io File.cpp 
      FileInputStream.cpp 
      FileOutputStream.cpp 
      CharArrayInputStream.cpp 
      CharArrayOutputStream.cpp 
      BufferedOutputStream.cpp 
      BufferedInputStream.cpp 
      ObjectOutputStream.cpp 
      ObjectInputStream.cpp
      StringInputStream.cpp
      StringOutputStream.cpp
      QueueOutputStream.cpp
      QueueInputStream.cpp
)
TARGET_LINK_LIBRARIES(io
    ${Boost_LIBRARIES}
    lang
    safmq
    util
    net
)
IF(BUILD_SHARED_LIBS)
INSTALL(TARGETS io
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
ENDIF(BUILD_SHARED_LIBS)

