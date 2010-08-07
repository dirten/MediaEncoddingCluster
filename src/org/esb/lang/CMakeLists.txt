
include_directories (${BREAKPAD_INCLUDE_DIR})
#include_directories (/home/jhoelscher/curl/include)
LINK_DIRECTORIES(${BREAKPAD_LIB_DIR})

SET(lang_source
    Byte.cpp
    String.cpp
    Thread.cpp
    Exception.cpp
    StackDumper.cpp
    CtrlCHitWaiter.cpp
)
IF(NOT WIN32)
  SET(lang_source
    ${lang_source}
    LinuxCpuInfo.cpp
  )
ELSEIF(APPLE)

ELSEIF(WIN32)
  SET(lang_source
    ${lang_source}
    WindowsCpuInfo.cpp
  )

ENDIF(NOT WIN32)

add_library (lang
  ${lang_source}
#      lang/NoSuchElementException.cpp
#      lang/NullPointerException.cpp
)

FIND_LIBRARY(COCOA_LIBRARY COCOA)
MARK_AS_ADVANCED(COCOA_LIBRARY)

TARGET_LINK_LIBRARIES(lang ${BREAKPAD_LIBS} util)
IF(WIN32)
	TARGET_LINK_LIBRARIES(lang wininet)
ELSE(WIN32)
	TARGET_LINK_LIBRARIES(lang dl)
  IF(APPLE)
    TARGET_LINK_LIBRARIES(lang crypto ${COCOA_LIBRARY})
  ENDIF(APPLE)

ENDIF(WIN32)
TARGET_LINK_LIBRARIES(lang
  ${Boost_LIBRARIES}
)

IF(NOT WIN32)
	TARGET_LINK_LIBRARIES(lang
		pthread
	)
ENDIF(NOT WIN32)
IF(BUILD_SHARED_LIBS)
INSTALL(TARGETS lang
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
ENDIF(BUILD_SHARED_LIBS)
