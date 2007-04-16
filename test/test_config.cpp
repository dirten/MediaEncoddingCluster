//
// C++ Implementation: test_config
//
// Description: 
//
//
// Author: jhoelscher <jhoelscher@linux>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "org/esb/config/config.h"
using namespace org::esb::config;

int main(int argc, char**argv){
    Config::init("./cluster.cfg");
    cout << "Config:"<<Config::getConfig("src.path")<<endl;;
}

