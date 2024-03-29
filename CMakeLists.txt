############################################################
#
# Main Cmake file
# @TODO: need to clean up the build system!!!
############################################################
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project (MEC)


############################################################
#
# Setup the version for MHive
#
############################################################
SET(MAJOR_VERSION 0)
SET(MINOR_VERSION 5)
SET(PATCH_VERSION 2)
SET(MINOR_PATCH_VERSION 0)
SET(REV ${SVN_VERSION})
SET(MAC_OSX_MIN_VERSION 10.7)




if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE MATCHES "")
  message(STATUS "Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

IF(UNIX)
  SET(LINUX)
ENDIF(UNIX)

INCLUDE(cmake/CMakeListExternals.txt)
INCLUDE(CTest)
INCLUDE(cmake/HiveFindGo.txt)
#INCLUDE(cmake/CMakeListExternals.txt)
INCLUDE(cmake/Go.cmake)
#INCLUDE(cmake/DownloadDeps.txt)

LINK_DIRECTORIES(${BOOST_DIR}/lib)

add_definitions(-DPOCO_NO_AUTOMATIC_LIBS)
add_definitions(-DFF_API_ALLOC_CONTEXT)
add_definitions(-DFF_API_AVCODEC_OPEN)
add_definitions(-DFF_API_OLD_DECODE_AUDIO)
add_definitions(-DFF_API_OLD_ENCODE_AUDIO)
add_definitions(-DFF_API_CODEC_ID)
#ADD_DEFINITIONS(-DPOCO_NO_AUTOMATIC_LIBS)
ADD_DEFINITIONS(-DPOCO_STATIC)

#add_definitions(-Weffc++)
#add_definitions(-Wall)

#SET(CMAKE_OSX_DEPLOYMENT_TARGET 10.6)

IF(NOT WIN32)
  add_definitions(-fPIC)
  set(CMAKE_CXX_FLAGS "-D_REENTRANT -Wl,-Bsymbolic")
  set(CMAKE_C_FLAGS "-D_REENTRANT -Wl,-Bsymbolic")
ENDIF(NOT WIN32)

IF(UNIX AND NOT APPLE)
  SET( CMAKE_SHARED_LINKER_FLAGS  "${CMAKE_SHARED_LINKER_FLAGS}  -Wl,-Bsymbolic" )
ENDIF(UNIX AND NOT APPLE)
IF(WIN32)
  add_definitions(-D_WIN32_WINNT=0x0501)
  ADD_DEFINITIONS(-DBOOST_USE_WINDOWS_H)
  #SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++" )
  SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wl,-Bsymbolic" )
  SET( CMAKE_SHARED_LINKER_FLAGS  ${CMAKE_SHARED_LINKER_FLAGS}  "-Wl,-Bsymbolic" )
  add_definitions(-ftemplate-depth=200)
ENDIF(WIN32)
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fpermissive" )



#SET(CMAKE_COLOR_MAKEFILE Off)

if(APPLE)
#add_definitions(-m32)
#add_definitions(-arch i386)
#SET (CMAKE_EXE_LINKER_FLAGS "-arch i386")
  add_definitions(-ftemplate-depth=200)
#  add_definitions(-m32)
  add_definitions(-mmacosx-version-min=${MAC_OSX_MIN_VERSION})

  #add_definitions(-std=c++11)

  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -mmacosx-version-min=${MAC_OSX_MIN_VERSION})
  set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -mmacosx-version-min=${MAC_OSX_MIN_VERSION})
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -mmacosx-version-min=${MAC_OSX_MIN_VERSION}")
  set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} -mmacosx-version-min=${MAC_OSX_MIN_VERSION})
ENDIF(APPLE)

SET(SOCORRO_HOST localhost CACHE PATH "Socorro Host")
SET(INSTALLBUILDER_EXE "/usr/bin/builder" CACHE PATH "Path to the Bitrock Installbuilder Executable")

#IF(UNIX)
#    EXECUTE_PROCESS(COMMAND chmod +x ${PROJECT_SOURCE_DIR}/target/dependency/bin/protoc)
#ENDIF(UNIX)


ENABLE_TESTING()
SET(DART_TESTING_TIMEOUT 100)


SET(CURL_CMD curl)
SET(ZIP_CMD gzip)
SET(MHIVE_DBGSYM mhive)
SET(DUMPSYM_CMD "${BREAKPAD_DIR}/bin/dump_syms")
IF(WIN32)
  SET(CURL_CMD "${PROJECT_SOURCE_DIR}/win32/curl.exe")
  SET(ZIP_CMD "${PROJECT_SOURCE_DIR}/win32/gzip.exe")
  SET(DUMPSYM_CMD "${PROJECT_SOURCE_DIR}/win32/dump_syms.exe")
  SET(MHIVE_DBGSYM mhive.pdb)
ENDIF(WIN32)

#set(CTEST_MEMORYCHECK_COMMAND_OPTIONS "${CTEST_MEMORYCHECK_COMMAND_OPTIONS} --num-callers=50") 
set(VALGRIND_COMMAND_OPTIONS "-q --tool=memcheck --leak-check=yes --show-reachable=yes --workaround-gcc296-bugs=yes --num-callers=50")

IF(BUILD_TESTING)
  SET(BUILDNAME "${BUILDNAME}" CACHE STRING "Name of build on the dashboard")
  MARK_AS_ADVANCED(BUILDNAME)
ENDIF(BUILD_TESTING)

IF(UNIX)
  set(CMAKE_CXX_FLAGS "-D_REENTRANT")
  set(CMAKE_C_FLAGS "-D_REENTRANT")
ENDIF(UNIX)
#SET(USE_EMBEDDED_MYSQL ON)

#INCLUDE(cmake/HiveFindBoost.txt)

#INCLUDE(FindBoost.cmake)

#set(Boost_DEBUG true)
set(BOOST_ROOT ${BOOST_DIR})
#set(Boost_COMPILER ${BOOST_COMPILER})
#set(Boost_USE_STATIC_LIBS On)
#find_package( Boost 1.44.0 COMPONENTS date_time program_options signals thread system filesystem serialization)
#SET(Boost_LIBRARIES libboost_date_time.a libboost_system.a libboost_filesystem.a libboost_serialization.a libboost_program_options.a)
#if(WIN32)
#    SET(Boost_LIBRARIES ${Boost_LIBRARIES} libboost_thread_win32.a)
#ELSE(LINUX)
#    SET(Boost_LIBRARIES ${Boost_LIBRARIES} libboost_thread.a)
#ENDIF(WIN32)

#if(Boost_FOUND)
#    MESSAGE("*** New Boost found" ${Boost_LIBRARIES})
#    MESSAGE("*** Boost Date Time found" ${BOOST_DATE_TIME_LIB})
#    set(BOOST_DT_LIB_MT ${BOOST_DT_LIB})
#endif()
#MESSAGE("Boost Include Dir" ${BOOST_INCLUDE_DIRS})       

#INCLUDE(cmake/HiveFindSQLite.txt)
#INCLUDE(cmake/HiveFindCurl.txt)
#INCLUDE(cmake/HiveFindFFmpeg.txt)
#INCLUDE(cmake/HiveFindBreakpad.txt)
#INCLUDE(cmake/HiveFindProtobuf.txt)
#INCLUDE(cmake/HiveFindLibJson.txt)
#INCLUDE(cmake/HiveFindPoco.txt)


#add_custom_target(
#	set_svn_revision
#	COMMAND echo "SET\\(MHIVE_SVN_REVISION `svnversion`\\)" > svn_revision.txt
#    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
#)
#INCLUDE(svn_revision.txt)




############################################################
#
# Build Type Options
#
############################################################



IF(CMAKE_BUILD_TYPE MATCHES "Debug")
  ADD_DEFINITIONS(-DDEBUG)
ENDIF(CMAKE_BUILD_TYPE MATCHES "Debug")
IF(CMAKE_BUILD_TYPE MATCHES "release")
  SET(CMAKE_BUILD_TYPE RelWithDebInfo)
  MESSAGE(${CMAKE_BUILD_TYPE})
ENDIF(CMAKE_BUILD_TYPE MATCHES "release")
############################################################
#
# Settings for the CPAck Packaging
# on windows the packages will be build with the BitrockInstaller setup
#
############################################################
SET(CPACK_PACKAGE_VERSION_MAJOR "${MAJOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_MINOR "${MINOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}.${MINOR_PATCH_VERSION}")
SET(CPACK_PACKAGE_NAME "MediaEncodingCluster")
SET(CPACK_PACKAGE_VENDOR "EsbLab")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "EsbLab Installer")
SET(CPACK_RPM_PACKAGE_DEBUG 1)
SET(CPACK_PACKAGE_INSTALL_DIRECTORY MediaEncodingCluster)
SET(CPACK_INCLUDE_TOPLEVEL_DIRECTORY Off)
#SET(CPACK_STRIP_FILES mhive)
SET(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/LICENSE)
IF(WIN32)
  SET(CPACK_GENERATOR "NSIS" )
  SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
      CreateShortCut \\\"$SMPROGRAMS\\\\\$STARTMENU_FOLDER\\\\Start Mhive Console.lnk\\\" \\\"$INSTDIR\\\\bin\\\\mhive.exe\\\" -c
  ")
ELSEIF(APPLE)
  #SET(CPACK_PACKAGE_DEFAULT_LOCATION "/Applications/MediaEncodingCluster")
  #SET(CPACK_PACKAGING_INSTALL_PREFIX "/")
  SET(CPACK_BUNDLE_PLIST "${PROJECT_SOURCE_DIR}/Info.plist")
  SET(CPACK_BUNDLE_STARTUP_COMMAND "${PROJECT_SOURCE_DIR}/src/mectl")
  SET(CPACK_BUNDLE_NAME "MediaEncodingCluster")
  SET(CPACK_BUNDLE_ICON "${PROJECT_SOURCE_DIR}/test.icns")
  #SET(CPACK_POSTFLIGHT_SCRIPT ${PROJECT_SOURCE_DIR}/install.osx)
  #SET(CPACK_GENERATOR "Bundle" )
  SET(CPACK_GENERATOR "TGZ" )
ELSE(WIN32)
  SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Jan Hoelscher")
  SET(CPACK_GENERATOR "ZIP" )
ENDIF(WIN32)

set(CPACK_SOURCE_IGNORE_FILES
"~$"
"\\\\.cvsignore$"
"^${PROJECT_SOURCE_DIR}.*/CVS/"
"^${PROJECT_SOURCE_DIR}/debian/"
"^${PROJECT_SOURCE_DIR}/old/"
"^${PROJECT_SOURCE_DIR}/build/"
"^${PROJECT_SOURCE_DIR}/install/"
)
############################################################
#
# Find the libbz2, it is needed for ffmpeg when visible
#
############################################################
#FIND_LIBRARY(LIBBZ2
#  NAMES
#    bz2
#  PATHS
#    /usr/lib
#  )
#IF(LIBBZ2)
#  SET(BZ2_LIB ${LIBBZ2})
#ENDIF(LIBBZ2)
############################################################
#
# Find the libssl, if it is needed for wt when visible
#
############################################################
#FIND_LIBRARY(LIBSSL
#  NAMES
#    ssl
#  PATHS
#    /usr/lib
#  )
#IF(LIBSSL)
#  SET(SSL_LIB ${LIBSSL})
#ENDIF(LIBSSL)
############################################################
#
# Find the libcrypto, if it is needed for wt when visible
#
############################################################
#FIND_LIBRARY(LIBCRYPTO
#  NAMES
#    crypto
#  PATHS
#    /usr/lib
#  )
#IF(LIBCRYPTO)
#  SET(CRYPTO_LIB ${LIBCRYPTO})
#ENDIF(LIBCRYPTO)
############################################################
#
# Setup build Defines
#
############################################################
#SET(BUILD_SHARED_LIBS Off)
IF(UNIX)
  ADD_DEFINITIONS(-D__UNIX__)
  IF(APPLE)
    ADD_DEFINITIONS(-D__APPLE__)
  ELSE(APPLE)
    ADD_DEFINITIONS(-D__LINUX__)
  ENDIF(APPLE)
ELSEIF(WIN32)
  ADD_DEFINITIONS(-D__WIN32__)
ENDIF(UNIX)
ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS)



#LINK_DIRECTORIES(${MYSQL_LIBRARY_PATH})
#LINK_DIRECTORIES(${WT_DIR}/lib)

#SET(BUILD_EMBEDDED_LIBS OFF CACHE STRING "Use Embedded Dependency Libraries")
#SET(USE_EMBEDDED_MYSQL ON CACHE STRING "Use Embedded Mysql")

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH}:$ORIGIN/../lib")







#INCLUDE(cmake/HiveBuildBoost.txt)
#INCLUDE(cmake/HiveBuildWT.txt)
#INCLUDE(cmake/HiveBuildX264.txt)
#INCLUDE(cmake/HiveBuildFfmpeg.txt)



#LINK_DIRECTORIES(${FFMPEG_DIR}/lib)


ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)


#INCLUDE(cmake/HiveFindGloox.txt)
#INCLUDE(cmake/HiveFindZeromq.txt)

IF(WIN32)
#  INSTALL(FILES "c:/vcredist_x86.exe" DESTINATION . )
  SET( ISS_SOURCE_PATH ${CMAKE_INSTALL_PREFIX})
#  CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/iss/setup.iss.in ${PROJECT_SOURCE_DIR}/iss/setup.iss)
ENDIF(WIN32)

CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/iss/mhive.xml.in ${PROJECT_SOURCE_DIR}/iss/mhive.xml @ONLY)


SET(DEFAULT_DATABASE_HOST "127.0.0.1")
SET(DEFAULT_DATABASE "sqlite3")
SET(MHIVE_VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}.${MINOR_PATCH_VERSION})

add_subdirectory (src)
add_subdirectory (test)


INCLUDE(CPack)



