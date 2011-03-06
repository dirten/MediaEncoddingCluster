/* 
 * File:   GridNode.cpp
 * Author: HoelscJ
 * 
 * Created on 3. Dezember 2010, 15:30
 */

#include "GridNode.h"
//#include "boost/date_time/posix_time/posix_time.hpp"
namespace org {
  namespace esb {
    namespace grid {

      GridNode::GridNode() {
        _name = "null";
        _status = NODE_UP;
        //_last_activity = boost::posix_time::second_clock::local_time();
      }

      GridNode::GridNode(boost::asio::ip::udp::endpoint & ep, std::string data) {
        _ep = ep;
        _ipaddress = ep.address();
        _name = "null";
        //_last_activity = boost::posix_time::second_clock::local_time();
        _status = NODE_UP;
      }
      
      std::string GridNode::toString() {
        std::ostringstream oss;
        oss << _ep.address() << ":";
        oss << _ep.port();
        //        oss<<":"<<_ep.protocol();
        //oss << ":" << boost::posix_time::to_simple_string(_last_activity);
        return oss.str();
      }
        GridNode::NodeStatus GridNode::getStatus(){
          return _status;
        }

      std::string GridNode::getData(std::string key) {
        return _node_data[key];
      }

      void GridNode::setData(std::string key, std::string value) {
        _node_data[key] = value;
      }

      void GridNode::setEndpoint(boost::asio::ip::udp::endpoint ep) {
        _ep = ep;
        //        _ipaddress = ep.address();
      }

      const boost::asio::ip::address GridNode::getIpAddress()const {
        return _ep.address();
      }

      bool GridNode::operator==(const GridNode&a)const {
        //        logdebug("&operator==");
        return _ep == a._ep;
      }

      bool GridNode::operator==(const GridNode*a)const {
        //        logdebug("*operator==");
        return _ep == a->_ep;
      }

      GridNode::~GridNode() {
      }
    }
  }
}
