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
#include "org/esb/util/StringTokenizer.h"
#include "org/esb/lang/Thread.h"
using namespace org::esb::config;
using namespace org::esb::util;
using namespace org::esb::lang;

int main(int argc, char**argv){
//    unsigned char * tmp=new unsigned char[1024*1024*1024];
//    bzero(tmp,1024*1024*1024);
//    Thread::sleep(10000);
    Config::init("./cluster.cfg");
    cout << "Config:"<<Config::getProperty("src.path")<<endl;;

    StringTokenizer * st=new StringTokenizer("test string"," ");
    cout << "Tokens:"<<st->countTokens()<<endl;
}

