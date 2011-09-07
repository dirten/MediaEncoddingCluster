
include_directories (${MEC_SOURCE_DIR})
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${BOOST_INCLUDE_DIRS})
include_directories (${FFMPEG_INCLUDE_DIR})
include_directories (${LOG4CPLUS_DIR}/include/litesql)




add_library (jobcreator SHARED
  JobUtil.cpp
  CodecPropertyTransformer.cpp
  PresetReaderJson.cpp
 )

TARGET_LINK_LIBRARIES(jobcreator
  json
)

SET_TARGET_PROPERTIES( jobcreator PROPERTIES PREFIX "")



INSTALL(TARGETS jobcreator
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
  #ARCHIVE DESTINATION plugins
)
