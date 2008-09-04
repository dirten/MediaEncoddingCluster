#ifndef ORG_ESB_WEB_FILEINFO
#define ORG_ESB_WEB_FILEINFO
#include <Wt/WContainerWidget>
namespace org{
namespace esb{
namespace web{
  class FileInfo:public Wt::WContainerWidget{
    public:
      FileInfo(int file_id, Wt::WContainerWidget * parent=0):Wt::WContainerWidget(parent){
        
      }
  };
}}}
#endif

