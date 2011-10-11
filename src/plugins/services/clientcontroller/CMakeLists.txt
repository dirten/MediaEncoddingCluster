
include_directories (${MEC_SOURCE_DIR})
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${FFMPEG_INCLUDE_DIR})




add_library (clientcontroller SHARED
  Service.cpp
 )

TARGET_LINK_LIBRARIES(clientcontroller
  core
)

SET_TARGET_PROPERTIES( clientcontroller PROPERTIES PREFIX "")


#installation currently disabled
INSTALL(TARGETS clientcontroller
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
)
