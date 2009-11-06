/*----------------------------------------------------------------------
 *  File    : JobScanner.h
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose :
 *  Created : 6. November 2009, 12:30 by Jan Hölscher <jan.hoelscher@esblab.com>
 *
 *
 * MediaEncodingCluster, Copyright (C) 2001-2009   Jan Hölscher
 *
 * This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 *
 * ----------------------------------------------------------------------
 */

#include "org/esb/signal/Message.h"
#include "org/esb/signal/MessageListener.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/PreparedStatement.h"
//#include "boost/thread.hpp"
#include <boost/thread/condition.hpp>

#ifndef _JOBSCANNER_H
#define	_JOBSCANNER_H

namespace org {
    namespace esb {
        namespace hive {

            class JobScanner : public org::esb::signal::MessageListener {
            public:
                JobScanner();
                ~JobScanner();
                void start();
                void stop();
                void onMessage(org::esb::signal::Message & msg);
            private:
                void run();
                org::esb::sql::Connection * _con;
                org::esb::sql::PreparedStatement * _stmt;
                boost::mutex terminationMutex;
                boost::condition termination_wait;

                bool _run;
                bool _running;
                int _interval;
            };
        }
    }
}


#endif	/* _JOBSCANNER_H */

