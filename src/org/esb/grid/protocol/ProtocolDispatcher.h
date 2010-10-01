/* 
 * File:   ProtocolDispatcher.h
 * Author: HoelscJ
 *
 * Created on 1. Oktober 2010, 14:32
 */

#ifndef PROTOCOLDISPATCHER_H
#define	PROTOCOLDISPATCHER_H
#include <string>
namespace org {
  namespace esb {
    namespace grid {

      class ProtocolDispatcher {
      public:
        ProtocolDispatcher();
        ProtocolDispatcher(const ProtocolDispatcher& orig);
        void dispatch(std::string datacmd);
        virtual ~ProtocolDispatcher();
      private:

      };
    }
  }
}

#endif	/* PROTOCOLDISPATCHER_H */

