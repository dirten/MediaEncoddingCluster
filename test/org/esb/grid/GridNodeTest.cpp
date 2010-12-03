/* 
 * File:   GridNodeTest.cpp
 * Author: HoelscJ
 *
 * Created on 3. Dezember 2010, 15:39
 */

#include <cstdlib>
#include "org/esb/grid/GridNode.h"
#include "org/esb/util/Log.h"
using namespace std;
using namespace org::esb::grid;
/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();
  GridNode node_a;
  GridNode node_b;
  LOGDEBUG(node_a.toString());

  return 0;
}

