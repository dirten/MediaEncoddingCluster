#include "hivecontrol.h"

using namespace org::esb::hive;
using namespace org::esb::config;

HiveControl::HiveControl(){
    Config::init("cluster.cfg");

}


