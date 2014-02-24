
include_directories (${BREAKPAD_INCLUDE_DIR})
#include_directories (/home/jhoelscher/curl/include)
LINK_DIRECTORIES(${BREAKPAD_LIB_DIR})

SET(lang_source
    Byte.cpp
    String.cpp
    Thread.cpp
    Exception.cpp
#    StackDumper.cpp
#    StackDumpUploader.cpp
    CtrlCHitWaiter.cpp
    ProcessException.cpp
    ProcessListener.cpp
    ProcessSupervisor.cpp
    NotFoundException.cpp
    Object.cpp
)
IF(NOT WIN32)
  SET(lang_source
    ${lang_source}
#    posix/SystemCpuInfo.cpp
    posix/Process.cpp
    posix/SharedObjectLoader.cpp
  )
ELSEIF(APPLE)

ELSEIF(WIN32)
  SET(lang_source
    ${lang_source}
#    win/SystemCpuInfo.cpp
    win/Process.cpp
    win/SharedObjectLoader.cpp
  )

ENDIF(NOT WIN32)

add_library (lang SHARED
  ${lang_source}
#      lang/NoSuchElementException.cpp
#      lang/NullPointerException.cpp
)

FIND_LIBRARY(COCOA_LIBRARY COCOA)
MARK_AS_ADVANCED(COCOA_LIBRARY)

TARGET_LINK_LIBRARIES(lang
#    ${BREAKPAD_LIBS}
#    util
)
TARGET_LINK_LIBRARIES(lang
  ${Boost_LIBRARIES}
  ${POCO_LIB_FOUNDATION}
  ${POCO_LIB_NET_UTIL}
)

IF(WIN32)
	TARGET_LINK_LIBRARIES(lang wininet iphlpapi)
ELSE(WIN32)
	TARGET_LINK_LIBRARIES(lang dl)
  IF(APPLE)
    TARGET_LINK_LIBRARIES(lang crypto ${COCOA_LIBRARY})
  ELSE(APPLE)
  TARGET_LINK_LIBRARIES(lang
    rt
  )
  ENDIF(APPLE)

ENDIF(WIN32)
TARGET_LINK_LIBRARIES(lang
  ${Boost_LIBRARIES}
  ${POCO_LIB_FOUNDATION}

)

IF(NOT WIN32)
	TARGET_LINK_LIBRARIES(lang
		pthread
	)
ENDIF(NOT WIN32)
#IF(BUILD_SHARED_LIBS)
INSTALL(TARGETS lang
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
#    ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)
#ENDIF(BUILD_SHARED_LIBS)
