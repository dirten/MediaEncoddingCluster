ADD_DEFINITIONS(-D_FILE_OFFSET_BITS=64)
ADD_DEFINITIONS(-D__WXGTK__)


include_directories (/home/HoelscJ/qtsdk-2010.05/qt/include)
include_directories (${MEC_SOURCE_DIR}/src)
include_directories (${MEC_SOURCE_DIR}/target/dependency/include/wx-2.9)
include_directories (${MEC_SOURCE_DIR}/target/dependency/lib/wx/include/gtk2-unicode-static-2.9)
include_directories (${MEC_SOURCE_DIR}/target/dependency/lib/wx/include/osx_carbon-unicode-static-2.9)
include_directories (${MEC_SOURCE_DIR}/target/dependency/include)
LINK_DIRECTORIES(${MEC_SOURCE_DIR}/target/dependency/lib)
LINK_DIRECTORIES(/home/HoelscJ/qtsdk-2010.05/lib)


add_executable (mhivectrl CtrlWindow.cpp)
#add_executable (qtwindow QtWindow.cpp)
target_link_libraries (mhivectrl wx_osx_carbonu-2.9)
#target_link_libraries (qtwindow QtGui QtCore)


#/home/HoelscJ/devel/mec/source/build/wxwidgets/lib/libwx_baseu_xml-2.9.a /home/HoelscJ/devel/mec/source/build/wxwidgets/lib/libwx_baseu_net-2.9.a /home/HoelscJ/devel/mec/source/build/wxwidgets/lib/libwx_baseu-2.9.a