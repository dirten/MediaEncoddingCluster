/*----------------------------------------------------------------------
 *  File    : ExportScanner.h
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose : it watches for completed files and export them to disk
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

#ifndef _ORG_ESB_HIVE_EXPORTSCANNER_H
#define	_ORG_ESB_HIVE_EXPORTSCANNER_H

#include "org/esb/signal/MessageListener.h"
#include "org/esb/signal/Message.h"
#include <boost/thread/condition.hpp>
#include "org/esb/util/Log.h"
namespace org {
    namespace esb {
        namespace hive {

            class ExportScanner : public org::esb::signal::MessageListener {
              classlogger("org.esb.hive.ExportScanner")
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

