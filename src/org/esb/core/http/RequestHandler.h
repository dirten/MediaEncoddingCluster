/* 
 * File:   RequestHandler.h
 * Author: HoelscJ
 *
 * Created on 24. Januar 2012, 11:07
 */

#ifndef REQUESTHANDLER_H
#define	REQUESTHANDLER_H
#include "Poco/Net/HTTPRequestHandler.h"
namespace org{
  namespace esb{
    namespace core{
      namespace http{
        class RequestHandler: public Poco::Net::HTTPRequestHandler{
          
        };
      }
    }
  }
}



#endif	/* REQUESTHANDLER_H */

