/* 
 * File:   GridNode.cpp
 * Author: HoelscJ
 * 
 * Created on 3. Dezember 2010, 15:30
 */

#include "GridNode.h"
#include "org/esb/util/Foreach.h"
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
        oss << _ep.port()<<" ";
        typedef std::map<std::string, std::string> KeyValueMap;
        foreach(KeyValueMap::value_type line, _node_data){
          oss<<line.first<<"="<<line.second<<" ";
        }
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

      bool GridNode::operator==(const GridNode & rhs)const {
        return _ep.address() == rhs._ep.address();
      }


      bool GridNode::operator==(const GridNode * rhs)const {
        return _ep.address() == rhs->_ep.address();
      }

      boost::posix_time::ptime GridNode::getLastActivity(){
        return _last_activity;
      }
      void GridNode::setLastActivity(boost::posix_time::ptime la){
        _last_activity=la;
      }

      void GridNode::setStatus(NodeStatus status){
        _status=status;
      }

      GridNode::~GridNode() {
      }
    }
  }
}
