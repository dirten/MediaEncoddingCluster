#include "ConnectionManager.h"
#include "org/esb/config/config.h"
namespace org{
  namespace esb{
    namespace sql{
      
      Connection ConnectionManager::getConnection(std::string name){
        using namespace org::esb::config;
        return Connection(std::string(Config::getProperty("db.connection")));
      }
    }
  }
}

