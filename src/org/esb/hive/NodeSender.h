/*----------------------------------------------------------------------
 *  File    : NodeSender.h
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose :
 *  Created : 7. Dezember 2009, 14:34 by Jan Hölscher <jan.hoelscher@esblab.com>
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


#ifndef _NODESENDER_H
#define	_NODESENDER_H
#include "boost/asio.hpp"
#include "org/esb/signal/MessageListener.h"
#include "org/esb/signal/Message.h"
namespace org {
    namespace esb {
        namespace hive {

            class NodeSender : public org::esb::signal::MessageListener {
            public:
                NodeSender(const boost::asio::ip::address& multicast_address);
                void start();
                void stop();
                void onMessage(org::esb::signal::Message&);
            private:
                boost::asio::io_service io_service_;
                boost::asio::ip::udp::endpoint endpoint_;
                boost::asio::ip::udp::socket socket_;
                boost::asio::deadline_timer timer_;
                int message_count_;
                std::string message_;
                void handle_send(const boost::system::error_code& error);
                void handle_timeout(const boost::system::error_code& error);
            };

        };

    }
}


#endif	/* _NODESENDER_H */

