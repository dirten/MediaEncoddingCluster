SET(GOROOT $ENV{GOROOT})
FIND_PROGRAM(GO_COMPILER
    bin/6g
    bin/7g
    bin/8g
    bin/6g.exe
    bin/7g.exe
    bin/8g.exe
  PATHS
    ${GOROOT}
  NO_DEFAULT_PATH
)
FIND_PROGRAM(GO_LINKER
    bin/6l
    bin/7l
    bin/8l
    bin/6l.exe
    bin/7l.exe
    bin/8l.exe
  PATHS
    ${GOROOT}
  NO_DEFAULT_PATH
)

FIND_PROGRAM(GO_PACK
    bin/gopack
    bin/gopack.exe
  PATHS
    ${GOROOT}
  NO_DEFAULT_PATH
)


MACRO (add_go_executable exefile infiles)
    MESSAGE(STATUS "Process file: ${exefile} ${ARGV} ")
    #list(APPEND testparam ${infileName})
    SET(arguments)
    FOREACH(infileName ${ARGV} )
        SET(exc 0)
        IF(${exefile} STREQUAL ${infileName})
            SET(exc 1)
        ENDIF(${exefile} STREQUAL ${infileName})
        IF(NOT ${exc})
            list(APPEND testparam ${infileName})
        ENDIF(NOT ${exc})
    ENDFOREACH(infileName)
    ADD_CUSTOM_COMMAND(OUTPUT ${exefile}.8
                       COMMAND ${GO_COMPILER}
                       ARGS -I ${CMAKE_CURRENT_BINARY_DIR}/pkg -o ${CMAKE_CURRENT_BINARY_DIR}/${exefile}.8 ${testparam}
                       MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/${infiles}
                       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                       COMMENT "do $ENV{GOROOT}/bin/8g -o ${CMAKE_CURRENT_BINARY_DIR}/${exefile}.8 ${testparam}"
    )
    ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${exefile}
                       COMMAND ${GO_LINKER}
                       ARGS -L ${CMAKE_CURRENT_BINARY_DIR}/pkg -o ${CMAKE_CURRENT_BINARY_DIR}/${exefile} ${exefile}.8
                       MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/${infiles}
                       WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                       COMMENT "do $ENV{GOROOT}/bin/8l -o ${CMAKE_CURRENT_BINARY_DIR}/${exefile} ${exefile}.8"
    )
    ADD_CUSTOM_TARGET("${exefile}.exe" ALL DEPENDS ${exefile}.8 ${exefile})
    SET(testparam "")
ENDMACRO (add_go_executable)

MACRO (target_link_go_libraries exefile infiles)
    MESSAGE(STATUS "Process link library : ${exefile} ${ARGV} ")
    SET(arguments)
    FOREACH(infileName ${ARGV} )
        SET(exc 0)
        IF(${exefile} STREQUAL ${infileName})
            SET(exc 1)
        ENDIF(${exefile} STREQUAL ${infileName})
        IF(NOT ${exc})
            ADD_DEPENDENCIES(${exefile}.exe ${infiles})
#            ADD_DEPENDENCIES(${infiles} ${exefile})
        ENDIF(NOT ${exc})
    ENDFOREACH(infileName)
    SET(mytest bla)
ENDMACRO (target_link_go_libraries )

MACRO (add_go_library libfile infiles)
    MESSAGE(STATUS "Process Library file: ${libfile} ${ARGV} ")
    SET(arguments)
    FOREACH(infileName ${ARGV} )
        SET(exc 0)
        IF(${libfile} STREQUAL ${infileName})
            SET(exc 1)
        ENDIF(${libfile} STREQUAL ${infileName})
        IF(NOT ${exc})
            list(APPEND testparam ${infileName})
        ENDIF(NOT ${exc})
    ENDFOREACH(infileName)
    ADD_CUSTOM_COMMAND(OUTPUT ${libfile}.8
                       COMMAND ${GO_COMPILER}
                       ARGS -o ${CMAKE_CURRENT_BINARY_DIR}/${libfile}.8 ${testparam}
                       MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/${infiles}
                       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                       COMMENT "do $ENV{GOROOT}/bin/8g -o ${CMAKE_CURRENT_BINARY_DIR}/${libfile}.8 ${testparam}"
    )
    ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${libfile}.a
                       COMMAND ${GO_PACK}
                       ARGS grc ${CMAKE_CURRENT_BINARY_DIR}/${libfile}.a ${libfile}.8
                       MAIN_DEPENDENCY ${CMAKE_CURRENT_BINARY_DIR}/${libfile}.8
                       WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                       COMMENT "do $ENV{GOROOT}/bin/gopack -o ${CMAKE_CURRENT_BINARY_DIR}/${libfile} ${libfile}.8"
    )
    list(APPEND library_directory "-I ${CMAKE_CURRENT_BINARY_DIR}")
    ADD_CUSTOM_TARGET("${libfile}" ALL DEPENDS ${libfile}.8 ${libfile}.a)
    set(testparam "")

ENDMACRO (add_go_library)
