/* 
 * File:   RapidXmlTest.cpp
 * Author: HoelscJ
 *
 * Created on 13. Oktober 2010, 14:04
 */

#include <cstdlib>
#include "boost/property_tree/detail/rapidxml.hpp"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/util/Log.h"
using namespace std;
using namespace org::esb::io;
using namespace rapidxml;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  std::string path = MEC_SOURCE_DIR;

//  FileInputStream fis(path+"/test/test.props");
  FileInputStream fis(path+"/res/comp/test.gui");
  std::string data;
  fis.read(data);


  xml_document<> doc;
  char * testdata=const_cast<char*>(data.c_str());
  doc.parse<0>(testdata);
  for(xml_node<>*node=doc.first_node("guis");node;node=node->next_sibling()){
    LOGDEBUG("Node"<<node->name());
  }


  return 0;
}

