/* 
 * File:   ConnectionManager.h
 * Author: jhoelscher
 *
 * Created on 6. November 2008, 14:42
 */

#include "Connection.h"

#include <string>
#ifndef _CONNECTIONMANAGER_H
#define	_CONNECTIONMANAGER_H
namespace org{
  namespace esb{
    namespace sql{
      class ConnectionManager{
      public:
        static Connection getConnection(std::string name);
      };
    }
  }
}


#endif	/* _CONNECTIONMANAGER_H */

