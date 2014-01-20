/* 
 * File:   GridNode.h
 * Author: HoelscJ
 *
 * Created on 3. Dezember 2010, 15:30
 */

#ifndef GRIDNODE_H
#define	GRIDNODE_H
#include <boost/asio.hpp>
#include <boost/serialization/binary_object.hpp>
#include <boost/serialization/map.hpp>
namespace org {
  namespace esb {
    namespace grid {

      class GridNode {
      public:

        enum NodeStatus {
          NODE_UP,
          NODE_DOWN
        };

        GridNode();
        virtual ~GridNode();
        GridNode(boost::asio::ip::udp::endpoint & ep, std::string data);
        const boost::asio::ip::address getIpAddress()const;
        const boost::asio::ip::address getName()const;
        boost::asio::ip::udp::endpoint _ep;
        bool operator==(const GridNode & a)const;
        bool operator==(const GridNode * a)const;
        std::string toString();

        NodeStatus getStatus();
        std::string getData(std::string key);
        void setData(std::string key, std::string value);
        void setEndpoint(boost::asio::ip::udp::endpoint);
        boost::posix_time::ptime getLastActivity();
        void setLastActivity(boost::posix_time::ptime la);
        void setStatus(NodeStatus status);
        private:
        boost::asio::ip::address _ipaddress;
        std::string _name;
        NodeStatus _status;
        std::map<std::string, std::string> _node_data;
        friend class NodeResolver;
        boost::posix_time::ptime _last_activity;
      public:

        template<class Archive>
        void serialize(Archive & ar, const unsigned int version) {
          ar & _node_data;
        }

      };

    }
  }
}

#endif	/* GRIDNODE_H */

