/* 
 * File:   ExportScanner.h
 * Author: jhoelscher
 *
 * Created on 15. Dezember 2008, 18:01
 */

#ifndef _ORG_ESB_HIVE_EXPORTSCANNER_H
#define	_ORG_ESB_HIVE_EXPORTSCANNER_H
#include "org/esb/signal/MessageListener.h"
#include "org/esb/signal/Message.h"
#include <boost/thread/condition.hpp>

namespace org {
    namespace esb {
        namespace hive {

            class ExportScanner : public org::esb::signal::MessageListener {
            public:
                ExportScanner();
                void onMessage(org::esb::signal::Message & msg);
            private:
                void start();
                bool _run;
                boost::mutex terminationMutex;
                boost::condition termination_wait;

            };
        }
    }
}


#endif	/* _ORG_ESB_HIVE_EXPORTSCANNER_H */

