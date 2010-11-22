/* 
 * File:   GuiBuilderTest.c
 * Author: HoelscJ
 *
 * Created on 13. Oktober 2010, 12:40
 */

#include "org/esb/web/WebApp2.h"
#include "org/esb/web/GuiBuilder.h"
#include <stdio.h>
#include <stdlib.h>
#include "Wt/Test/WTestEnvironment"
#include "org/esb/config/config.h"
using namespace org::esb::web;

/*
 * 
 */
int main(int argc, char** argv) {
  Wt::Test::WTestEnvironment *env = new Wt::Test::WTestEnvironment();
  org::esb::web::WebApp2 app(*env);

  Log::open("");
  std::string path = MEC_SOURCE_DIR;

  std::map<std::string, std::string> data;

  GuiBuilder gui(path + "/res/comp/test.gui", data);

  org::esb::config::Config::close();
  
  return (EXIT_SUCCESS);
}

