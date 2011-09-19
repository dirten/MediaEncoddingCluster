/* 
 * File:   PresetVerifierTest.cpp
 * Author: HoelscJ
 *
 * Created on 19. September 2011, 13:49
 */

#include <cstdlib>
#include "plugins/hooks/jsonapi/PresetVerifier.h"
#include "org/esb/util/Log.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();
  JSONNode root(JSON_NODE);
  
  org::esb::plugin::PresetVerifier::verify(root);
  return 0;
}

