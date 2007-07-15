#include "BundleIndex.h"
#include <string.h>
using namespace org::esb::hive;
using namespace std;
int BundleIndex::VERSION=1;

BundleIndex::BundleIndex(){
    indexNumber=0;
    startPts=0;
    endPts=0;
    memset(bundleName,0,100);
}


