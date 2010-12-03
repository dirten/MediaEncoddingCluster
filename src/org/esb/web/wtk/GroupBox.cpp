/* 
 * File:   GroupBox.cpp
 * Author: HoelscJ
 * 
 * Created on 13. Oktober 2010, 16:43
 */

#include <list>


#include <vector>
#include <typeinfo>

#include "GroupBox.h"
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace web {

      GroupBox::GroupBox():Wt::WGroupBox() {
      }

      GroupBox::~GroupBox() {
      }

      void GroupBox::setHidden(bool b){
        
        Wt::WGroupBox::setHidden(b);
        Wt::WGroupBox::setDisabled(b);
      }
      std::list<std::string> GroupBox::getChildrenObjectNames(){
        std::list<std::string> result;
        std::vector<Wt::WWidget*>childs=children();
        std::vector<Wt::WWidget*>::iterator it=childs.begin();
        for(;it!=childs.end();it++){
          result.push_back((*it)->objectName());
          LOGDEBUG("Children name="<<(*it)->objectName());
        }
        return result;
      }
    }
  }
}
