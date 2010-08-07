#include "FilterPanelFactory.h"

#include "ResizeFilterPanel.h"
#include "DeinterlaceFilterPanel.h"
namespace org{
  namespace esb{
    namespace web{
      std::set<BaseFilterPanel *> FilterPanelFactory::_filter_map;
      FilterPanelFactory::FilterPanelFactory(){}
      FilterPanelFactory::~FilterPanelFactory(){}
      BaseFilterPanel * FilterPanelFactory::getFilterPanel(std::string id){
          if(id=="resize")return new ResizeFilterPanel();
          if(id=="deinterlace")return new DeinterlaceFilterPanel();
          return NULL;
      }
    }
  }
}

