
include_directories (${BREAKPAD_INCLUDE_DIR})
LINK_DIRECTORIES(${BREAKPAD_LIB_DIR})

add_library (lang
    Byte.cpp
    String.cpp
    Thread.cpp
    Exception.cpp
    StackDumper.cpp
#      lang/NoSuchElementException.cpp
#      lang/NullPointerException.cpp
)
FIND_LIBRARY(COCOA_LIBRARY COCOA)
MARK_AS_ADVANCED(COCOA_LIBRARY)

IF(WIN32)
	TARGET_LINK_LIBRARIES(lang exception_handler)
ELSE(WIN32)
	TARGET_LINK_LIBRARIES(lang breakpad)
  IF(APPLE)
    TARGET_LINK_LIBRARIES(lang crypto ${COCOA_LIBRARY})
  ENDIF(APPLE)

ENDIF(WIN32)
TARGET_LINK_LIBRARIES(lang
  ${BOOST_THREAD_LIB_MT}
)
