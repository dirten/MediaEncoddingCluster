
include_directories (${MEC_SOURCE_DIR})
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${FFMPEG_INCLUDE_DIR})




add_library (uploadervice SHARED
  UploadService.cpp
 )

TARGET_LINK_LIBRARIES(uploadervice
  core
)

SET_TARGET_PROPERTIES( uploadervice PROPERTIES PREFIX "")


#installation currently disabled
INSTALL(TARGETS uploadervice
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
)
