/* 
 * File:   GridNodeServer.cpp
 * Author: HoelscJ
 * 
 * Created on 1. Oktober 2010, 13:31
 */

#include "GridNodeServer.h"
#include "boost/bind.hpp"
namespace org {
  namespace esb {
    namespace grid {

      
      GridNodeServer::GridNodeServer() : recv_socket_(recv_service_) {
        boost::asio::ip::udp::endpoint listen_endpoint(boost::asio::ip::address::from_string("0.0.0.0"), 20200);
        recv_socket_.open(listen_endpoint.protocol());
        recv_socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
        recv_socket_.bind(listen_endpoint);

        // Join the multicast group.
        recv_socket_.set_option(
                boost::asio::ip::multicast::join_group(boost::asio::ip::address::from_string("239.255.0.1")));

        recv_socket_.async_receive_from(
                boost::asio::buffer(data_, max_length), recv_endpoint_,
                boost::bind(&GridNodeServer::handle_receive, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
        recv_service_.run();
      }

      GridNodeServer::~GridNodeServer() {

      }

      void GridNodeServer::handle_receive(const boost::system::error_code& error, size_t bytes_recvd) {
        LOGDEBUG("Bytes recv:"<<bytes_recvd<<" Data:"<<data_);
        
        recv_socket_.async_receive_from(
                boost::asio::buffer(data_, max_length), recv_endpoint_,
                boost::bind(&GridNodeServer::handle_receive, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
      }
    }
  }
}
