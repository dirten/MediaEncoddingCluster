#include <Wt/WWidget>
#include <Wt/WContainerWidget>

#include "SqlTable.h"
namespace org{
namespace esb{
namespace web{
class Profiles: public Wt::WContainerWidget{
  public:
    Profiles(Wt::WContainerWidget * parent=0):Wt::WContainerWidget(parent) {
      new SqlTable(std::string("select * from profiles"), this);
    }
};
}}}

