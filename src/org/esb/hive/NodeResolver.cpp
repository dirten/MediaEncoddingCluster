
#include <list>

/*----------------------------------------------------------------------
 *  File    : NodeResolver.h
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose :
 *  Created : 8. Dezember 2009, 12:36 by Jan Hölscher <jan.hoelscher@esblab.com>
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
#include "NodeResolver.h"

#include <boost/bind.hpp>
#include <boost/thread.hpp>

namespace org {
  namespace esb {
    namespace hive {

      Node::Node(boost::asio::ip::udp::endpoint & ep){
        _ep=ep;
        _ipaddress=ep.address();
        _name="null";
        _last_activity=boost::posix_time::second_clock::local_time();
      }
      const boost::asio::ip::address Node::getIpAddress()const{
        return _ipaddress;
      }
      
      bool Node::operator==(const Node&a)const {
//        logdebug("&operator==");
        return _ep==a._ep;
      }
      bool Node::operator==(const Node*a)const {
//        logdebug("*operator==");
        return _ep==a->_ep;
      }


      NodeResolver::NodeResolver(const boost::asio::ip::address& listen_address,const boost::asio::ip::address& multicast_address, int port) :
      send_endpoint_(multicast_address, port),
      send_socket_(send_service_, send_endpoint_.protocol()),
      send_timer_(send_service_),
      recv_socket_(recv_service_) {
        send_socket_.async_send_to(
            boost::asio::buffer(message_), send_endpoint_,
            boost::bind(&NodeResolver::handle_send, this,
            boost::asio::placeholders::error));

        boost::asio::ip::udp::endpoint listen_endpoint(listen_address, port);
        recv_socket_.open(listen_endpoint.protocol());
        recv_socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
        recv_socket_.bind(listen_endpoint);

        // Join the multicast group.
        recv_socket_.set_option(
            boost::asio::ip::multicast::join_group(multicast_address));

        recv_socket_.async_receive_from(
            boost::asio::buffer(data_, max_length), recv_endpoint_,
            boost::bind(&NodeResolver::handle_receive, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));

      }

      void NodeResolver::handle_send(const boost::system::error_code& error) {
        if (!error) {
          send_timer_.expires_from_now(boost::posix_time::seconds(1));
          send_timer_.async_wait(
              boost::bind(&NodeResolver::handle_send_timeout, this,
              boost::asio::placeholders::error));
        }


      }

      void NodeResolver::handle_send_timeout(const boost::system::error_code& error) {

        if (!error) {
          std::ostringstream os;
          os << "server ";
          message_ = os.str();

          send_socket_.async_send_to(
              boost::asio::buffer(message_), send_endpoint_,
              boost::bind(&NodeResolver::handle_send, this,
              boost::asio::placeholders::error));
        }

      }

      void NodeResolver::handle_receive(const boost::system::error_code& error, size_t bytes_recvd) {
        if (!error) {
          boost::shared_ptr<Node> nodePtr=boost::shared_ptr<Node>(new Node(recv_endpoint_));
//          boost::shared_ptr<Node> nodePtr2=boost::shared_ptr<Node>(new Node(recv_endpoint_));
//          Node n1(recv_endpoint_);
//          Node n2(recv_endpoint_);
          bool contains=false;
          std::list<boost::shared_ptr<Node> >::iterator it=_nodes.begin();
          for(;it!=_nodes.end();it++){
            boost::posix_time::ptime actual_time=boost::posix_time::second_clock::local_time();
            if((*it)->_last_activity+boost::posix_time::seconds(4)<actual_time){
              logdebug("Node TimeOut:"<<(*it));
              _nodes.remove((*it));
            }
            if(*(*it)==*nodePtr){
              contains=true;
              (*it)->_last_activity=actual_time;
//              break;
            }
          }
          if(!contains){
            logdebug("New Node found"<<recv_endpoint_);
            _nodes.push_back(nodePtr);
          }
          recv_socket_.async_receive_from(
              boost::asio::buffer(data_, max_length), recv_endpoint_,
              boost::bind(&NodeResolver::handle_receive, this,
              boost::asio::placeholders::error,
              boost::asio::placeholders::bytes_transferred));
        } else {
          logerror( "error" << error.message());
        }
      }

      void NodeResolver::start() {
        boost::thread recv_thread(boost::bind(&boost::asio::io_service::run, &recv_service_));
        boost::thread send_thread(boost::bind(&boost::asio::io_service::run, &send_service_));
      }

      void NodeResolver::setNodeListener(NodeListener & listener){

      }
    }
  }
}
