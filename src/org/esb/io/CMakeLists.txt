


#include_directories (${MEC_SOURCE_DIR}/src/org/esb/io)
add_library (io File.cpp 
      FileInputStream.cpp 
      FileOutputStream.cpp 
      CharArrayInputStream.cpp 
      CharArrayOutputStream.cpp 
      BufferedOutputStream.cpp 
      BufferedInputStream.cpp 
      ObjectOutputStream.cpp 
      ObjectInputStream.cpp
)
TARGET_LINK_LIBRARIES(io
  ${BOOST_FS_LIB_MT}
  ${BOOST_SYSTEM_LIB_MT}
  ${BOOST_SERIAL_LIB_MT}
  lang
)


