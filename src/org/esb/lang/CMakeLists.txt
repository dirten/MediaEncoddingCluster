
include_directories (${BREAKPAD_INCLUDE_DIR})
include_directories (/home/jhoelscher/curl/include)
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

TARGET_LINK_LIBRARIES(lang ${BREAKPAD_LIBS})
IF(WIN32)
	TARGET_LINK_LIBRARIES(lang crash_report_sender wininet)
ELSE(WIN32)
	TARGET_LINK_LIBRARIES(lang glog dl)
  IF(APPLE)
    TARGET_LINK_LIBRARIES(lang crypto ${COCOA_LIBRARY})
  ENDIF(APPLE)

ENDIF(WIN32)
TARGET_LINK_LIBRARIES(lang
  ${BOOST_THREAD_LIB_MT}
  pthread
)
