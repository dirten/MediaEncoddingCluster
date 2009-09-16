
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
IF(WIN32)
	TARGET_LINK_LIBRARIES(lang exception_handler)
ELSE(WIN32)
	TARGET_LINK_LIBRARIES(lang breakpad)
ENDIF(WIN32)
