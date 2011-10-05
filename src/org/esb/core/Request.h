/* 
 * File:   Request.h
 * Author: HoelscJ
 *
 * Created on 31. August 2011, 15:50
 */

#ifndef REQUEST_H
#define	REQUEST_H
#include <boost/shared_ptr.hpp>
namespace org {
  namespace esb {
    namespace core {

      class Request {
      public:
        void setProperty(std::string key, boost::shared_ptr<void> );
        boost::shared_ptr<void> getProperty(std::string key);
        virtual ~Request(){};
      };
    }
  }
}

#endif	/* REQUEST_H */

