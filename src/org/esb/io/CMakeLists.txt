#SET(SHARED_IO On)

IF(SHARED_IO)
    SET(IO_LINK_TYPE SHARED)
    ADD_DEFINITIONS(-DIO_BUILD_SHARED)
ELSE(SHARED_IO)
    SET(IO_LINK_TYPE STATIC)
ENDIF(SHARED_IO)

MESSAGE("IO Build Type" ${LINK_TYPE})

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
      portable_binary_iarchive.cpp
      portable_binary_oarchive.cpp
)
#MESSAGE("Boost Libraries in io " ${Boost_LIBRARIES})

TARGET_LINK_LIBRARIES(io
  lang
  #${Boost_LIBRARIES}
    #mq
    #lang
    #util
)
SET_TARGET_PROPERTIES( io PROPERTIES PREFIX "libmhive-")
IF(SHARED_IO)
INSTALL(TARGETS io
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
#    ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)
ENDIF(SHARED_IO)

