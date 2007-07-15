#include "BundleIndexWriter.h"
#include "org/esb/io/FileOutputStream.h"


using namespace org::esb::hive;

BundleIndexWriter::BundleIndexWriter(const char * index_name){
    os=new FileOutputStream(index_name);
    os->write((char*)&BundleIndex::VERSION, sizeof(int));
}

void BundleIndexWriter::writeIndex(BundleIndex & index){
    os->write((char*)&index, sizeof(BundleIndex));
}

