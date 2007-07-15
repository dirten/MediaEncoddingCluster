#ifndef ORG_ESB_HIVE_PACKETINDEX_H
#define ORG_ESB_HIVE_PACKETINDEX_H
//#include "avformat.h"
#include <stdint.h>
namespace org{
namespace esb{
namespace hive{
class BundleIndex{
    public:
	BundleIndex();
	int indexNumber;
	int64_t startPts;
	int64_t endPts;
	char bundleName[100];
	static int VERSION;
};
}}}
#endif

