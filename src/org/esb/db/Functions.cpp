
#include "hivedb.hpp"
#include <string>

namespace db{
  std::string Job::getStatusText(){
    std::vector < std::pair< std::string, std::string > >::iterator it=status_values.begin();
    std::ostringstream oss;
    oss << status;
    std::string val=oss.str();
    for(;it!=status_values.end();it++){
      if((*it).second==val){
        return (*it).first;
      }
    }
    return "";
  }
  std::string Task::getStatusText(){
    std::vector < std::pair< std::string, std::string > >::iterator it=status_values.begin();
    std::ostringstream oss;
    oss << status;
    std::string val=oss.str();
    for(;it!=status_values.end();it++){
      if((*it).second==val){
        return (*it).first;
      }
    }
    return "";
  }
}
