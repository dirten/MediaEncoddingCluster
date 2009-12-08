/*----------------------------------------------------------------------
 *  File    : NodeSender.cpp
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
#include "NodeSender.h"
#include <boost/bind.hpp>
#include <boost/thread.hpp>

namespace org {
  namespace esb {
    namespace hive {

      NodeSender::NodeSender(const boost::asio::ip::address& multicast_address) :
      endpoint_(multicast_address, 5011),
      socket_(io_service_, endpoint_.protocol()),
      timer_(io_service_),
      message_count_(0) {
        std::ostringstream os;
        os << "server " << message_count_++;
        message_ = os.str();

        socket_.async_send_to(
            boost::asio::buffer(message_), endpoint_,
            boost::bind(&NodeSender::handle_send, this,
            boost::asio::placeholders::error));

      }

      void NodeSender::handle_send(const boost::system::error_code& error) {
        if (!error && message_count_ < 10) {
          timer_.expires_from_now(boost::posix_time::seconds(1));
          timer_.async_wait(
              boost::bind(&NodeSender::handle_timeout, this,
              boost::asio::placeholders::error));
        }


      }

      void NodeSender::handle_timeout(const boost::system::error_code& error) {

        if (!error) {
          std::ostringstream os;
          os << "server " << message_count_++;
          message_ = os.str();

          socket_.async_send_to(
              boost::asio::buffer(message_), endpoint_,
              boost::bind(&NodeSender::handle_send, this,
              boost::asio::placeholders::error));
        }

      }

      void NodeSender::start() {
        boost::thread t(boost::bind(&boost::asio::io_service::run,&io_service_));
      }
      void NodeSender::stop() {

      }

      void NodeSender::onMessage(org::esb::signal::Message&msg) {

      }
    }
  }
}

