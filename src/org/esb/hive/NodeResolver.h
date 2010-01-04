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


#ifndef _NODERESOLVER_H
#define	_NODERESOLVER_H
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <list>
#include "org/esb/util/Log.h"
#include "boost/date_time/posix_time/posix_time.hpp"
namespace org {
  namespace esb {
    namespace hive {

      class Node {
      public:
        Node(boost::asio::ip::udp::endpoint & ep);
        const boost::asio::ip::address getIpAddress()const;
        const boost::asio::ip::address getName()const;
        boost::asio::ip::udp::endpoint _ep;
        bool operator==(const Node & a)const;
        bool operator==(const Node * a)const;
        std::string toString();
        enum NODE_STATUS {
          NODE_UP,
          NODE_DOWN
        };
        NODE_STATUS _status;

      private:
        boost::asio::ip::address _ipaddress;
        std::string _name;
        friend class NodeResolver;
        boost::posix_time::ptime _last_activity;
      };

      class NodeListener {
      public:
        virtual void onNodeUp(Node & node) = 0;
        virtual void onNodeDown(Node & node) = 0;

      };

      class NodeResolver {
      public:
        typedef std::list<boost::shared_ptr<Node> > NodeList;
        NodeResolver(const boost::asio::ip::address& listen_address, const boost::asio::ip::address& multicast_address, int);
        void setNodeListener(NodeListener * listener);
        void start();
        void stop();
        void setNodeTimeout(unsigned int sec);
        void setNodeMessage(std::string msg);
        NodeList getNodes();


      private:
        boost::asio::io_service send_service_;
        boost::asio::io_service recv_service_;

        boost::asio::ip::udp::endpoint send_endpoint_;
        boost::asio::ip::udp::endpoint recv_endpoint_;

        boost::asio::ip::udp::socket send_socket_;
        boost::asio::ip::udp::socket recv_socket_;

        boost::asio::deadline_timer send_timer_;

        enum {
          max_length = 1024
        };
        char data_[max_length];

        int message_count_;
        std::string message_;

        NodeList _nodes;
        std::list<NodeListener*>_listener;

        std::string _message;
        unsigned int _node_timeout;
        void handle_send(const boost::system::error_code& error);
        void handle_send_timeout(const boost::system::error_code& error);
        void handle_receive(const boost::system::error_code& error, size_t bytes_recvd);

        enum NotifyType {
          NODE_UP,
          NODE_DOWN
        };
        void notifyListener(Node & n);

      };
    }
  }
}

#endif	/* _NODERESOLVER_H */

