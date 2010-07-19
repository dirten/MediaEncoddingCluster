/* 
 * File:   ProcessUnitBuilder.h
 * Author: HoelscJ
 *
 * Created on 16. Juli 2010, 17:10
 */

#ifndef PROCESSUNITBUILDER_H
#define	PROCESSUNITBUILDER_H
#include "StreamData.h"
#include "ProcessUnit.h"
#include "boost/shared_ptr.hpp"
#include<map>
namespace org {
  namespace esb {
    namespace hive {
      namespace job {

        class ProcessUnitBuilder {

        public:
          ProcessUnitBuilder(map<int, StreamData>&);
          ProcessUnitBuilder(const ProcessUnitBuilder&);
          virtual ~ProcessUnitBuilder();
          boost::shared_ptr<ProcessUnit> build(org::esb::av::PacketListPtr);
        private:
          map<int, StreamData> & _map_data;
        };

      }
    }
  }
}

#endif	/* PROCESSUNITBUILDER_H */

