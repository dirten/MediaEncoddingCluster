/*----------------------------------------------------------------------
 *  File    : NodeReceiver.h
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose :
 *  Created : 7. Dezember 2009, 14:37 by Jan Hölscher <jan.hoelscher@esblab.com>
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


#ifndef _NODERECEIVER_H
#define	_NODERECEIVER_H
#include "org/esb/signal/MessageListener.h"
#include "org/esb/signal/Message.h"
#include <boost/asio.hpp>
namespace org {
    namespace esb {
        namespace hive {

            class NodeReceiver : public org::esb::signal::MessageListener {
            public:
                NodeReceiver(
                        const boost::asio::ip::address& listen_address,
                        const boost::asio::ip::address& multicast_address);

                void onMessage(org::esb::signal::Message&);
                void start();
                void stop();
            private:
                boost::asio::io_service io_service_;
                boost::asio::ip::udp::socket socket_;
                boost::asio::ip::udp::endpoint sender_endpoint_;

                enum {
                    max_length = 1024
                };
                char data_[max_length];

                void handle_receive(const boost::system::error_code& error, size_t bytes_recvd);
            };

        };
    }
}


#endif	/* _NODERECEIVER_H */

