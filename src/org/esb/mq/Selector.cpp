#include "Selector.h"
#include "QueueMessage.h"
#include "org/esb/util/StringTokenizer.h"
namespace org{
  namespace esb{
    namespace mq{
      Selector::Selector(std::string sel){
        org::esb::util::StringTokenizer tok(sel, "=");
        if(tok.countTokens()==2){
          _name=tok.nextToken();
          _value=tok.nextToken();
        }
        std::cout << "name="<<_name<<" value="<<_value<<std::endl;
      }
      Selector::Selector(){}

      bool Selector::match(QueueMessage & msg){
        bool result=false;
        //LOGDEBUG("Label="<<msg.getLabel()<<" Value="<<_value);
        std::cout << "label="<<msg.getLabel()<< " value="<<_value<<std::endl;
        if(_value.length()==0||msg.getLabel()==_value)
          result=true;
        return result;
      }
    }
  }
}

