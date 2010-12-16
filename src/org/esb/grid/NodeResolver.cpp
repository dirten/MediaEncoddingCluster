

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
#include <iostream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <fstream>
#include <sstream>
#include <exception>


//#include "boost/date_time/gregorian/gregorian.hpp"

namespace org {
  namespace esb {
    namespace grid {
      NodeResolver::NodeList NodeResolver::_nodes;

      Node::Node() {
        _name = "null";
        _status = NODE_UP;
        _last_activity = boost::posix_time::second_clock::local_time();
      }

      Node::Node(boost::asio::ip::udp::endpoint & ep, std::string data) {
        _ep = ep;
        _ipaddress = ep.address();
        _name = "null";
        _last_activity = boost::posix_time::second_clock::local_time();
        _status = NODE_UP;

      }

      std::string Node::toString() {
        std::ostringstream oss;
        oss << _ep.address() << ":";
        oss << _ep.port();
        //        oss<<":"<<_ep.protocol();
        oss << ":" << boost::posix_time::to_simple_string(_last_activity);
        return oss.str();
      }

      Node::NodeStatus Node::getStatus() {
        return _status;
      }

      std::string Node::getData(std::string key) {
        return _node_data[key];
      }

      void Node::setData(std::string key, std::string value) {
        _node_data[key] = value;
      }

      void Node::setEndpoint(boost::asio::ip::udp::endpoint ep) {
        _ep = ep;
        //        _ipaddress = ep.address();
      }

      const boost::asio::ip::address Node::getIpAddress()const {
        return _ep.address();
      }

      bool Node::operator==(const Node&a)const {
        //        logdebug("&operator==");
        return _ep == a._ep;
      }

      bool Node::operator==(const Node*a)const {
        //        logdebug("*operator==");
        return _ep == a->_ep;
      }

      NodeResolver::NodeResolver(const boost::asio::ip::address& listen_address, const boost::asio::ip::address& multicast_address, int port, Node node) :
      _node_timeout(5),
      send_endpoint_(multicast_address, port),
      send_socket_(send_service_, send_endpoint_.protocol()),
      send_timer_(send_service_),
      recv_socket_(recv_service_) {
        memset(&data_, 0, max_length);
        _self = node;
        std::ostringstream archive_stream;
        boost::archive::binary_oarchive archive(archive_stream);
        archive << _self;
        message_ = archive_stream.str();
        //        LOGDEBUG("Message size:"<<message_.size());
        send_socket_.async_send_to(
                boost::asio::buffer(message_), send_endpoint_,
                boost::bind(&NodeResolver::handle_send_size, this,
                boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

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

      void NodeResolver::setNode(Node node) {
      }

      void NodeResolver::handle_send(const boost::system::error_code& error) {
        if (!error) {
          send_timer_.expires_from_now(boost::posix_time::seconds(1));
          send_timer_.async_wait(
                  boost::bind(&NodeResolver::handle_send_timeout, this,
                  boost::asio::placeholders::error));
        }else{
          LOGERROR("Error send:"<<error.message());
        }
      }

      void NodeResolver::handle_send_size(const boost::system::error_code& error, std::size_t bytes_transferred) {
        //LOGDEBUG("bytes transfered:"<<bytes_transferred);
        if (!error) {
          send_timer_.expires_from_now(boost::posix_time::seconds(1));
          send_timer_.async_wait(
                  boost::bind(&NodeResolver::handle_send_timeout, this,
                  boost::asio::placeholders::error));
        }else{
          LOGERROR("Error send_size:"<<error.message());
        }
      }

      void NodeResolver::handle_send_timeout(const boost::system::error_code& error) {

        if (!error) {

          send_socket_.async_send_to(
                  boost::asio::buffer(message_), send_endpoint_,
                  boost::bind(&NodeResolver::handle_send_size, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        }else{
          LOGERROR("Error send_timeout:"<<error.message());
        }

      }

      void NodeResolver::handle_receive(const boost::system::error_code& error, size_t bytes_recvd) {
        //LOGDEBUG("Bytes received"<<bytes_recvd);
        if (!error) {
          Ptr<Node> nodePtr = Ptr<Node > (new Node());

          if (bytes_recvd > 0) {
            std::string tmp(data_, bytes_recvd);
            std::istringstream archive_stream(tmp);
            try {
              boost::archive::binary_iarchive archive(archive_stream);
              archive >> *nodePtr.get();
            } catch (std::exception & ex) {

              LOGERROR("Exception reading archive:" << ex.what());
            }
          }
          nodePtr->setEndpoint(recv_endpoint_);

          bool contains = false;
          std::list<Ptr<Node> >::iterator it = _nodes.begin();
          for (; it != _nodes.end(); it++) {
            boost::posix_time::ptime actual_time = boost::posix_time::second_clock::local_time();
            if ((*it)->_last_activity + boost::posix_time::seconds(_node_timeout) < actual_time) {
              if ((*it)->_status == Node::NODE_UP) {
                (*it)->_status = Node::NODE_DOWN;
                notifyListener(*(*it));
              }
            }
            if (*(*it) == *nodePtr) {
              contains = true;
              (*it)->_last_activity = actual_time;
            }
          }
          if (!contains) {
            _nodes.push_back(nodePtr);
            notifyListener(*nodePtr);
          }
          recv_socket_.async_receive_from(
                  boost::asio::buffer(data_, max_length), recv_endpoint_,
                  boost::bind(&NodeResolver::handle_receive, this,
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
        } else
          if (error != boost::system::errc::operation_canceled) {
          LOGERROR("error" << error.message());
        }
      }

      void NodeResolver::start() {
        std::list<Ptr<Node> >::iterator it = _nodes.begin();
        for (; it != _nodes.end(); it++) {
              notifyListener(*(*it));
        }
        boost::thread recv_thread(boost::bind(&boost::asio::io_service::run, &recv_service_));
        boost::thread send_thread(boost::bind(&boost::asio::io_service::run, &send_service_));
      }

      void NodeResolver::setNodeListener(NodeListener * listener) {
        _listener.push_back(listener);
      }

      void NodeResolver::setNodeTimeout(unsigned int sec) {
        _node_timeout = sec;
      }

      void NodeResolver::setNodeMessage(std::string message) {
        _message = message;
      }

      NodeResolver::NodeList NodeResolver::getNodes() {
        return _nodes;
      }

      void NodeResolver::stop() {
        recv_service_.stop();
        send_service_.stop();
      }

      void NodeResolver::notifyListener(Node & node) {
        //LOGDEBUG("Notify:"<<node.toString()<<":"<<node.getData("type"))
        boost::mutex::scoped_lock lock(notify_mutex);
        std::list<NodeListener*>::iterator it = _listener.begin();
        for (; it != _listener.end(); it++) {
          if (node._status == Node::NODE_UP) {
            LOGDEBUG("NotifyNodeUp:"<<node.toString())
            (*it)->onNodeUp(node);
          }
          if (node._status == Node::NODE_DOWN) {
            LOGDEBUG("NotifyNodeDown:"<<node.toString())
            (*it)->onNodeDown(node);
          }
        }
      }
    }
  }
}
