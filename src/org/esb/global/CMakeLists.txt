
SET(SHARED_GLOBAL On)

IF(SHARED_GLOBAL)
    ADD_DEFINITIONS(-DGLOBAL_BUILD_SHARED)
    SET(GLOBAL_LINK_TYPE SHARED)
ELSE(SHARED_GLOBAL)
    SET(GLOBAL_LINK_TYPE STATIC)
ENDIF(SHARED_GLOBAL)

add_library (api ${GLOBAL_LINK_TYPE}
  GlobalLibrary.cpp
 )


TARGET_LINK_LIBRARIES(global
    av
    json
    db
    util
    rpc
    hive
)

SET_TARGET_PROPERTIES( global PROPERTIES PREFIX "libmhive-")

