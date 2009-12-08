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

#include "NodeReceiver.h"
#include <boost/bind.hpp>
#include <boost/thread.hpp>

namespace org {
  namespace esb {
    namespace hive {

      NodeReceiver::NodeReceiver(
          const boost::asio::ip::address& listen_address,
          const boost::asio::ip::address& multicast_address) :
      socket_(io_service_) {
        boost::asio::ip::udp::endpoint listen_endpoint(
            listen_address, 5011);
        socket_.open(listen_endpoint.protocol());
        socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
        socket_.bind(listen_endpoint);

        // Join the multicast group.
        socket_.set_option(
            boost::asio::ip::multicast::join_group(multicast_address));

        socket_.async_receive_from(
            boost::asio::buffer(data_, max_length), sender_endpoint_,
            boost::bind(&NodeReceiver::handle_receive, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
      }

      void NodeReceiver::handle_receive(const boost::system::error_code& error, size_t bytes_recvd) {
        if (!error) {
          std::cout.write(data_, bytes_recvd);
          std::cout <<sender_endpoint_<<"-";
          std::cout << std::endl;

          socket_.async_receive_from(
              boost::asio::buffer(data_, max_length), sender_endpoint_,
              boost::bind(&NodeReceiver::handle_receive, this,
              boost::asio::placeholders::error,
              boost::asio::placeholders::bytes_transferred));
        }else{
          std::cout << "error"<<error<<std::endl;
        }
      }

      void NodeReceiver::onMessage(org::esb::signal::Message&msg) {

      }

      void NodeReceiver::start() {
        boost::thread t(boost::bind(&boost::asio::io_service::run,&io_service_));
      }

      void NodeReceiver::stop() {

      }

    }
  }
}
