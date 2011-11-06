#SET(SHARED_IO On)

IF(SHARED_IO)
    SET(IO_LINK_TYPE SHARED)
    ADD_DEFINITIONS(-DIO_BUILD_SHARED)
ELSE(SHARED_IO)
    SET(IO_LINK_TYPE STATIC)
ENDIF(SHARED_IO)
MESSAGE("IO Build Type" ${LINK_TYPE})

#include_directories (${MEC_SOURCE_DIR}/target/dependency/include/safmq)
add_library (io ${IO_LINK_TYPE} 
      File.cpp 
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
      #QueueOutputStream.cpp
      #QueueInputStream.cpp
)
TARGET_LINK_LIBRARIES(io
#    ${Boost_LIBRARIES}
    lang
    safmq
    util
)
SET_TARGET_PROPERTIES( io PROPERTIES PREFIX "libmhive-")
IF(SHARED_IO)
INSTALL(TARGETS io
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
#    ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)
ENDIF(SHARED_IO)

