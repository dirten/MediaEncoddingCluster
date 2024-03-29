include_directories (${MEC_SOURCE_DIR})
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${MYSQL_DIR}/include/mysql)
include_directories (${MYSQL_DIR}/include)
include_directories (${WT_DIR}/include)

add_executable (TestWebServer TestWebServer.cpp)
add_executable (TestPreviewFrameServer TestPreviewFrameServer.cpp)
add_executable (JobTableTest JobTableTest.cpp)
add_executable (PreviewFrameServerTest PreviewFrameServer.cpp)
add_executable (GuiBuilderTest GuiBuilderTest.cpp)
add_executable (TestWebApp TestWebApp.cpp)
add_executable (ProjectWizardTest ProjectWizardTest.cpp)


target_link_libraries (TestWebServer
  web
  hive
  av
  util
  signal
  io
  lang
  net
 )
target_link_libraries (TestPreviewFrameServer
  web
  hive
  av
  util
  signal
  io
  lang
  net
 )

target_link_libraries (JobTableTest
  web
  hive
  av
  util
  signal
  io
  lang
  net
 )
target_link_libraries (PreviewFrameServerTest
  web
  hive
  av
  util
  signal
  io
  lang
  net
 )
target_link_libraries (GuiBuilderTest
  web
  hive
  av
  util
  signal
  io
  lang
  net
 )
target_link_libraries (TestWebApp
  web
  hive
  av
  util
  signal
  io
  lang
  net
 )
target_link_libraries (ProjectWizardTest
  web
  hive
  av
  util
  signal
  io
  lang
  net
 )

target_link_libraries (TestWebServer
  ${BOOST_SERIAL_LIB_MT}
  ${BOOST_THREAD_LIB_MT}
  ${BOOST_SYSTEM_LIB_MT}
  ${BOOST_PO_LIB_MT}
  ${BOOST_SIGNALS_LIB_MT}
  ${BOOST_REGEX_LIB_MT}
  ${BOOST_FS_LIB_MT}
  ${BOOST_DT_LIB_MT}
)
ADD_TEST(TestWebServer ${EXECUTABLE_OUTPUT_PATH}/TestWebServer)
ADD_TEST(TestPreviewFrameServer ${EXECUTABLE_OUTPUT_PATH}/TestPreviewFrameServer)
#ADD_TEST(JobTableTest ${EXECUTABLE_OUTPUT_PATH}/JobTableTest)
ADD_TEST(web.PreviewFrameServerTest ${EXECUTABLE_OUTPUT_PATH}/PreviewFrameServerTest)
ADD_TEST(web.GuiBuilderTest ${EXECUTABLE_OUTPUT_PATH}/GuiBuilderTest)
ADD_TEST(web.ProjectWizardTest ${EXECUTABLE_OUTPUT_PATH}/ProjectWizardTest )

