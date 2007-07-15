#ifndef ORG_ESB_HIVE_BUNDLEINDEXWRITER_H
#define ORG_ESB_HIVE_BUNDLEINDEXWRITER_H
#include "BundleIndex.h"
#include "org/esb/io/OutputStream.h"

using namespace org::esb::io;
namespace org{
namespace esb{
namespace hive{
class BundleIndexWriter{
    public:
	BundleIndexWriter(const char * index_name);
	void writeIndex(BundleIndex & index);
    private:
	OutputStream * os;
};
}}}
#endif
