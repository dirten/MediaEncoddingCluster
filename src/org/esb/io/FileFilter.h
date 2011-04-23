#ifndef ORG_ESB_IO_FILEFILTER_H
#define ORG_ESB_IO_FILEFILTER_H
#include "exports.h"
namespace org{
namespace esb{
namespace io{
class File;
class IO_EXPORT FileFilter{
  public:
    virtual bool accept(File file)=0;
    virtual ~FileFilter(){}
};

}}}
#endif

