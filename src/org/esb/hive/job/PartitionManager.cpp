/* 
 * File:   PartitionManager.cpp
 * Author: HoelscJ
 * 
 * Created on 18. Juli 2011, 09:56
 */

#include "PartitionManager.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/io/StringOutputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "MQFactory.h"
#include "MessageQueue.h"
#include "safmq.h"

namespace org {
  namespace esb {
    namespace hive {
      PartitionManager * PartitionManager::_instance = NULL;

      PartitionManager * PartitionManager::getInstance() {
        if (_instance == NULL)
          _instance = new PartitionManager();
        _instance->createPartition("global", -1);
        return _instance;
      }

      PartitionManager::PartitionManager() {
        _con = new org::esb::mq::QueueConnection("safmq://admin:@localhost:20202");
      }

      PartitionManager::~PartitionManager() {
      }

      PartitionManager::Result PartitionManager::joinPartition(std::string name, boost::asio::ip::tcp::endpoint ep) {
        PartitionManager::Result result = PartitionManager::OK;
        if (_partition_map.find(name) != _partition_map.end()) {
          //if (_partition_sizes[name]>-1 && _partition_sizes[name] > _partition_map[name].size()) {
          _partition_map[name].push_back(ep);
          //} else {
          //result = PartitionManager::FULL;
          //}
        } else {
          result = PartitionManager::NOT_EXIST;
        }
        return result;
      }

      PartitionManager::Result PartitionManager::leavePartition(std::string name, boost::asio::ip::tcp::endpoint ep) {
        PartitionManager::Result result = PartitionManager::OK;

        return result;
      }

      PartitionManager::Result PartitionManager::createPartition(std::string name, int size) {
        PartitionManager::Result result = PartitionManager::OK;
        if (_partition_map.find(name) == _partition_map.end()) {
          _partition_map[name];
          _partition_sizes[name] = size;
        } else
          result = PartitionManager::EXIST;
        return result;
      }

      PartitionManager::Result PartitionManager::deletePartition(std::string name) {
        PartitionManager::Result result = PartitionManager::OK;
        if (_partition_map.find(name) != _partition_map.end()) {
          if (_partition_map[name].size() == 0) {
            _partition_map.erase(name);
          } else {
            result = PartitionManager::NOT_EMPTY;
          }
        } else
          result = PartitionManager::NOT_EXIST;
        return result;

      }

      void PartitionManager::putProcessUnit(std::string partition, boost::shared_ptr<job::ProcessUnit>unit) {
        int stream_index = unit->_source_stream;
        std::string queue_name = org::esb::util::StringUtil::toString(stream_index);
        if (_partition_streams.count(stream_index) == 0) {
          if (!_con->queueExist(queue_name))
            _con->createQueue(queue_name);
          _partition_streams[stream_index] = partition;
          _stream_max_endpoints[stream_index] = 0;
        }
        std::string data;
        org::esb::io::StringOutputStream sos(data);
        org::esb::io::ObjectOutputStream oos(&sos);
        oos.writeObject(*unit.get());
        org::esb::mq::QueueMessage msg;
        msg.setLabel("nolabel");
        msg.getBufferStream()->write(data.c_str(), data.length());
        safmq::ErrorCode er;
        /*
        if((er=_con->enqueue(queue_name, msg))!=safmq::EC_NOERROR){
          LOGERROR(er);
          throw std::exception();
        }*/

        _con->enqueue(queue_name, msg);

        //_partition_queue[partition];
      }

      int PartitionManager::getStream(boost::asio::ip::tcp::endpoint ep) {
        return 0;
      }
      /*
      boost::shared_ptr<job::ProcessUnit>PartitionManager::getProcessUnit(std::string partition,boost::asio::ip::tcp::endpoint ep) {
        //return _partition_queue[partition].dequeue();
      }*/
    }
  }
}
