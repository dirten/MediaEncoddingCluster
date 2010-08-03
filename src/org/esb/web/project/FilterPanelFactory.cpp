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
        BaseFilterPanel* result;
        _filter_map.clear();
          _filter_map.insert(new ResizeFilterPanel());
          _filter_map.insert(new DeinterlaceFilterPanel());
        
        std::set<BaseFilterPanel*>::iterator it=_filter_map.begin();
        for(;it!=_filter_map.end();it++){
          if(id==(*it)->getId())
            result=(*it);
        }
        return result;
      }
    }
  }
}

