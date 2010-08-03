#ifndef ORG_ESB_IO_FILEFILTER_H
#define ORG_ESB_IO_FILEFILTER_H
namespace org{
namespace esb{
namespace io{
class File;
class FileFilter{
  public:
    virtual bool accept(File & file)=0;
    virtual ~FileFilter(){}
};

}}}
#endif

