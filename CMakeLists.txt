############################################################
#
# Main Cmake file
#
############################################################
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project (MEC)


INCLUDE(CTest)
INCLUDE(cmake/HiveFindGo.txt)
INCLUDE(cmake/Go.cmake)
#INCLUDE(cmake/DownloadDeps.txt)

if(APPLE)
add_definitions(-m32)
add_definitions(-arch i386)
SET (CMAKE_EXE_LINKER_FLAGS "-arch i386")
ENDIF(APPLE)

if(WIN32)
add_definitions(/GX)
add_definitions(/EHsc)
ENDIF(WIN32)


SET(FFMPEG_DIR ${PROJECT_SOURCE_DIR}/target/dependency)
#SET(X264_DIR ${PROJECT_SOURCE_DIR}/target/dependency )
SET(BOOST_DIR ${PROJECT_SOURCE_DIR}/target/dependency)
#SET(BOOST_COMPILER gcc )
#SET(BOOST_VERSION "1_35" CACHE STRING "Boost libraries version signature")
SET(SQLITE_DIR ${PROJECT_SOURCE_DIR}/target/dependency)
SET(LOG4CPLUS_DIR ${PROJECT_SOURCE_DIR}/target/dependency )
SET(WT_DIR ${PROJECT_SOURCE_DIR}/target/dependency)
SET(BREAKPAD_DIR ${PROJECT_SOURCE_DIR}/target/dependency)
SET(SOCORRO_HOST localhost CACHE PATH "Socorro Host")
SET(INSTALLBUILDER_EXE "/usr/bin/builder" CACHE PATH "Path to the Bitrock Installbuilder Executable")
SET(CURL_DIR ${PROJECT_SOURCE_DIR}/target/dependency )

SET(LIBRARY_DIR ${PROJECT_SOURCE_DIR}/target/dependency/lib)
SET(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/target/dependency/include)
IF(UNIX)
    EXECUTE_PROCESS(COMMAND chmod +x ${PROJECT_SOURCE_DIR}/target/dependency/bin/protoc)
ENDIF(UNIX)


ENABLE_TESTING()
SET(DART_TESTING_TIMEOUT 100)

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
SET(USE_EMBEDDED_MYSQL ON)
#INCLUDE(cmake/HiveFindBoost.txt)

#INCLUDE(FindBoost.cmake)

#set(Boost_DEBUG true)
set(BOOST_ROOT ${BOOST_DIR})
set(Boost_COMPILER ${BOOST_COMPILER})
set(Boost_USE_STATIC_LIBS On)
find_package( Boost 1.36.0 COMPONENTS date_time regex program_options signals thread system filesystem serialization)
if(Boost_FOUND)
    MESSAGE("*** New Boost found" ${Boost_LIBRARIES})
#    MESSAGE("*** Boost Date Time found" ${BOOST_DATE_TIME_LIB})
    set(BOOST_DT_LIB_MT ${BOOST_DT_LIB})
endif()
#MESSAGE("Boost Include Dir" ${BOOST_INCLUDE_DIRS})       

INCLUDE(cmake/HiveFindSQLite.txt)
INCLUDE(cmake/HiveFindCurl.txt)
INCLUDE(cmake/HiveFindFFmpeg.txt)
INCLUDE(cmake/HiveFindBreakpad.txt)
INCLUDE(cmake/HiveFindProtobuf.txt)


#add_custom_target(
#	set_svn_revision
#	COMMAND echo "SET\\(MHIVE_SVN_REVISION `svnversion`\\)" > svn_revision.txt
#    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
#)
#INCLUDE(svn_revision.txt)


############################################################
#
# Setup the version for MHive
#
############################################################
SET(MAJOR_VERSION 0)
SET(MINOR_VERSION 0)
SET(PATCH_VERSION 4)
SET(MINOR_PATCH_VERSION 9)
SET(MHIVE_VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}.${MINOR_PATCH_VERSION})

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
# on windows the packages will be build with the inno setup
#
############################################################
SET(CPACK_PACKAGE_VERSION_MAJOR "${MAJOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_MINOR "${MINOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}.${MINOR_PATCH_VERSION}")
SET(CPACK_PACKAGE_NAME "MediaEncodingCluster")
SET(CPACK_PACKAGE_VENDOR "EsbLab")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY MediaEncodingCluster-${MHIVE_VERSION})
SET(CPACK_STRIP_FILES mhive)
IF(WIN32)
  SET(CPACK_GENERATOR "NSIS" )
#  SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS  
#      "File \\\"c:\\\\vcredist_x86.exe\\\"
#      ExecWait '\\\"$INSTDIR\\\\vcredist_x86.exe\\\" /q:a'
#      CreateShortCut \\\"$SMPROGRAMS\\\\\$STARTMENU_FOLDER\\\\Start Mhive.lnk" "$INSTDIR\\\\bin\\\\mhive.exe -s\\\"
#  ")
ELSEIF(APPLE)
  SET(CPACK_PACKAGE_DEFAULT_LOCATION "/Applications")
  SET(CPACK_PACKAGING_INSTALL_PREFIX "/")
  SET(CPACK_BUNDLE_PLIST "${PROJECT_SOURCE_DIR}/Info.plist")
  SET(CPACK_BUNDLE_STARTUP_COMMAND "${PROJECT_SOURCE_DIR}/src/mectl")
  SET(CPACK_BUNDLE_NAME "MediaEncodingCluster")
  SET(CPACK_BUNDLE_ICON "${PROJECT_SOURCE_DIR}/test.icns")
  SET(CPACK_GENERATOR "Bundle" )
  SET(CPACK_POSTFLIGHT_SCRIPT ${PROJECT_SOURCE_DIR}/install.osx)
  #SET(CPACK_GENERATOR "PackageMaker" )
ELSE(WIN32)
#  SET(CPACK_GENERATOR "TGZ;TBZ2;STGZ" )
ENDIF(WIN32)

############################################################
#
# Find the libbz2, it is needed for ffmpeg when visible
#
############################################################
FIND_LIBRARY(LIBBZ2
  NAMES
    bz2
  PATHS
    /usr/lib
  )
IF(LIBBZ2)
  SET(BZ2_LIB ${LIBBZ2})
ENDIF(LIBBZ2)
############################################################
#
# Find the libssl, if it is needed for wt when visible
#
############################################################
FIND_LIBRARY(LIBSSL
  NAMES
    ssl
  PATHS
    /usr/lib
  )
IF(LIBSSL)
  SET(SSL_LIB ${LIBSSL})
ENDIF(LIBSSL)
############################################################
#
# Find the libcrypto, if it is needed for wt when visible
#
############################################################
FIND_LIBRARY(LIBCRYPTO
  NAMES
    crypto
  PATHS
    /usr/lib
  )
IF(LIBCRYPTO)
  SET(CRYPTO_LIB ${LIBCRYPTO})
ENDIF(LIBCRYPTO)
############################################################
#
# Setup build Defines
#
############################################################
SET(BUILD_SHARED_LIBS Off)
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



LINK_DIRECTORIES(${MYSQL_LIBRARY_PATH})
LINK_DIRECTORIES(${WT_DIR}/lib)

SET(BUILD_EMBEDDED_LIBS OFF CACHE STRING "Use Embedded Dependency Libraries")
#SET(USE_EMBEDDED_MYSQL ON CACHE STRING "Use Embedded Mysql")

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH}:$ORIGIN/../lib")




# Build Embedded libraries in deps directory
IF(BUILD_EMBEDDED_LIBS)
  FILE(APPEND ${PROJECT_SOURCE_DIR}/deps/stage/boost.build "")
  FILE(APPEND ${PROJECT_SOURCE_DIR}/deps/stage/wt.build "")
  FILE(APPEND ${PROJECT_SOURCE_DIR}/deps/stage/ffmpeg.download "")
  FILE(APPEND ${PROJECT_SOURCE_DIR}/deps/stage/x264.download "")
  FILE(APPEND ${PROJECT_SOURCE_DIR}/deps/stage/mp3lame.download "")




#  MESSAGE("** Downloading erlang runtime")
#  FILE(DOWNLOAD http://erlang.org/download/otp_src_R13B.tar.gz bla/otp_src_R13B.tar.gz)

  MESSAGE("** Building Embedded Libraries, that will take several time")

  FILE(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/deps/stage)
  FILE(APPEND ${PROJECT_SOURCE_DIR}/deps/stage/boost.build "")
  FILE(APPEND ${PROJECT_SOURCE_DIR}/deps/stage/wt.build "")
  FILE(APPEND ${PROJECT_SOURCE_DIR}/deps/stage/ffmpeg.build "")
  FILE(APPEND ${PROJECT_SOURCE_DIR}/deps/stage/x264.build "")

# setup to build embedded libraries
  SET(BUILD_EMBEDDED_BOOST ON)
  INCLUDE(${PROJECT_SOURCE_DIR}/deps/stage/boost.build)
  SET(BUILD_EMBEDDED_WT ON)
  INCLUDE(${PROJECT_SOURCE_DIR}/deps/stage/wt.build)
  SET(BUILD_EMBEDDED_FFMPEG ON)
  INCLUDE(${PROJECT_SOURCE_DIR}/deps/stage/ffmpeg.build)
  SET(BUILD_EMBEDDED_X264 ON)
  INCLUDE(${PROJECT_SOURCE_DIR}/deps/stage/x264.build)

# settings for the embedded Library Paths
  SET(BOOST_DIR ${PROJECT_SOURCE_DIR}/deps/stage/boost)
  SET(BOOST_COMPILER gcc41 )
  SET(BOOST_VERSION "1_35" )
#  SET(FFMPEG_DIR ${PROJECT_SOURCE_DIR}/deps/stage/ffmpeg)
  SET(WT_DIR ${PROJECT_SOURCE_DIR}/deps/stage/wt)
  SET(X264_DIR ${PROJECT_SOURCE_DIR}/deps/stage/x264)
ENDIF(BUILD_EMBEDDED_LIBS)



#INCLUDE(cmake/HiveBuildBoost.txt)
#INCLUDE(cmake/HiveBuildWT.txt)
#INCLUDE(cmake/HiveBuildX264.txt)
#INCLUDE(cmake/HiveBuildFfmpeg.txt)

	




IF(NOT BUILD_EMBEDDED_LIBS)
SET(FFMPEG_DIR ${PROJECT_SOURCE_DIR}/target/dependency CACHE PATH "Use ffmpeg in directory")
#SET(X264_DIR ${PROJECT_SOURCE_DIR}/target/dependency CACHE PATH "Use x264 codec in directory")
SET(BOOST_DIR ${PROJECT_SOURCE_DIR}/target/dependency CACHE PATH "Boost installation path prefix")
SET(BOOST_COMPILER gcc CACHE STRING "Boost libraries compiler signature")
SET(BOOST_VERSION "1_35" CACHE STRING "Boost libraries version signature")
SET(MYSQL_DIR ${USERLIB_ROOT} CACHE PATH "Mysql libraries")
SET(LOG4CPLUS_DIR ${PROJECT_SOURCE_DIR}/target/dependency CACHE PATH "Log4cplus libraries")
SET(WT_DIR ${PROJECT_SOURCE_DIR}/target/dependency CACHE PATH "WebToolKit libraries")
SET(BREAKPAD_DIR ${PROJECT_SOURCE_DIR}/target/dependency CACHE PATH "Google Breakpad libraries")
SET(SOCORRO_HOST localhost CACHE PATH "Socorro Host")
SET(INSTALLBUILDER_EXE "/usr/bin/builder" CACHE PATH "Path to the Bitrock Installbuilder Executable")
SET(CURL_DIR ${PROJECT_SOURCE_DIR}/target/dependency CACHE PATH "Path to the Bitrock Installbuilder Executable")
ENDIF(NOT BUILD_EMBEDDED_LIBS)


#LINK_DIRECTORIES(${FFMPEG_DIR}/lib)


ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)


#INCLUDE(cmake/WtFindBoost.txt)

IF(WIN32)
  INSTALL(FILES "c:/vcredist_x86.exe" DESTINATION . )
  SET( ISS_SOURCE_PATH ${CMAKE_INSTALL_PREFIX})
#  CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/iss/setup.iss.in ${PROJECT_SOURCE_DIR}/iss/setup.iss)
ENDIF(WIN32)

CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/iss/mhive.xml.in ${PROJECT_SOURCE_DIR}/iss/mhive.xml @ONLY)


SET(DEFAULT_DATABASE_HOST "127.0.0.1")
SET(DEFAULT_DATABASE "sqlite3")
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_SOURCE_DIR}/config.h)
include_directories (${MYSQL_DIR}/include/litesql)
#include_directories (${MYSQL_DIR}/include/litesql/litesql)

add_subdirectory (src)
add_subdirectory (test)
#add_subdirectory (ice)
#IF(BUILD_TESTING)
#  IF(WIN32)
#    SET( ENV{PATH} "$ENV{PATH}\;${MYSQL_DIR}/Embedded/DLL/release\;${FFMPEG_DIR}/bin" )
#  ENDIF(WIN32)
#  add_subdirectory (tests)
#ENDIF(BUILD_TESTING)


INCLUDE(CPack)


