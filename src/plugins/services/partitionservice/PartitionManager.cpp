/* 
 * File:   PartitionManager.cpp
 * Author: HoelscJ
 * 
 * Created on 13. Juli 2011, 09:56
 */

#include "PartitionManager.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/io/StringOutputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/StringInputStream.h"
#include "org/esb/io/ObjectInputStream.h"
//#include "MQFactory.h"
//#include "MessageQueue.h"
//#include "safmq.h"
#include "org/esb/util/Foreach.h"
namespace partitionservice {
  PartitionManager * PartitionManager::_instance = NULL;

  /*
  bool PartitionManager::Endpoint::operator==(const PartitionManager::Endpoint&a)const {
    return ep == a.ep;
  }

  bool PartitionManager::Stream::operator==(const PartitionManager::Stream&a)const {
    return index == a.index;
  }
  bool PartitionManager::Partition::operator==(const PartitionManager::Partition&a)const {
    return name == a.name;
  }*/

  PartitionManager * PartitionManager::getInstance() {
    if (_instance == NULL)
      _instance = new PartitionManager();
    _instance->createPartition("global", -1);
    return _instance;
  }

  PartitionManager::PartitionManager() {
    //_con = new org::esb::mq::QueueConnection("safmq://admin:@localhost:20202");
  }

  PartitionManager::~PartitionManager() {
  }

  PartitionManager::Result PartitionManager::joinPartition(std::string name, boost::asio::ip::tcp::endpoint ep, Type type) {
    PartitionManager::Result result = PartitionManager::OK;
    if (_partitions.count(name) > 0) {
      _partitions[name].addEndpoint(Endpoint(ep));
    } else {
      result = PartitionManager::NOT_EXIST;
    }
    return result;
  }

  int PartitionManager::getSize(std::string partition) {
    /**
     * @TODO: implementing only Partition Size, currently there will be a count af all partitions
     * @param partition
     * @return 
     */
    int result = 0;
    typedef std::map<int, Ptr<org::esb::util::FileQueue<boost::shared_ptr<org::esb::hive::job::ProcessUnit> > > > S;

    foreach(S::value_type q, _stream_queues) {
      result += q.second->size();
    }
    return result;
  }

  PartitionManager::Result PartitionManager::leavePartition(std::string name, boost::asio::ip::tcp::endpoint ep) {
    PartitionManager::Result result = PartitionManager::OK;
    if (_endpoints.count(ep) > 0) {
      _endpoints.erase(ep);
    } else {
      result = PartitionManager::NOT_IN_PARTITION;
    }
    return result;
  }

  PartitionManager::Result PartitionManager::createPartition(std::string name, int size) {
    PartitionManager::Result result = PartitionManager::OK;
    if (_partitions.count(name) == 0) {
      Partition part(name);
      _partitions[name] = part;
    } else
      result = PartitionManager::EXIST;
    return result;
  }

  PartitionManager::Result PartitionManager::deletePartition(std::string name) {
    PartitionManager::Result result = PartitionManager::OK;
    if (_partitions.count(name) > 0) {
      bool to_delete = true;

      foreach(PartitionMap::value_type partition, _partitions) {

        foreach(Stream stream, partition.second.getStreams()) {
          if (stream.getEndpoints().size() > 0)
            to_delete = false;
        }
      }
      if (to_delete) {
        _partitions.erase(name);
      } else {
        result = PartitionManager::NOT_EMPTY;
      }
    } else
      result = PartitionManager::NOT_EXIST;
    return result;
  }

  void PartitionManager::putProcessUnit(std::string partition, boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit, Type type) {
    LOGDEBUG("PartitionManager::putProcessUnit partition=" << partition << " unitid=" << unit->_process_unit);
    if (unit->_input_packets.size() == 0)return;

    int stream_index = unit->_input_packets.front()->getStreamIndex();
    std::string stream_id = org::esb::util::StringUtil::toString(stream_index);

    if (_partitions.count(partition) > 0) {
      Partition part = _partitions[partition];
      if (!part.containStream(stream_id)) {
        Stream::TYPE stype = unit->_decoder->getCodecType() == AVMEDIA_TYPE_AUDIO ? Stream::ONE_FOR_ALL : Stream::ONE_FOR_ONE;
        part.addStream(Stream(stream_id, stype));
      }
      Stream stream = part.getStream(stream_id);
      Ptr<org::esb::hive::job::ProcessUnit>u(unit);
      stream.enqueue(u);
    }
  }

  boost::shared_ptr<org::esb::hive::job::ProcessUnit>PartitionManager::getProcessUnit(boost::asio::ip::tcp::endpoint ep) {
    boost::shared_ptr<org::esb::hive::job::ProcessUnit> result;

    LOGDEBUG("Endpoint=" << ep);

    /*search for partition*/
    foreach(PartitionMap::value_type partition, _partitions) {

      foreach(Partition::EndpointList::value_type ep, partition.second.getEndpoints()) {

      }

    }


    if (_endpoints.count(ep) > 0) {
      if (_endpoints[ep].stream == 0) {

        LOGDEBUG("endpoint have no associated stream, change this")
        foreach(StreamList::value_type & entry, _partition_stream_map[_endpoints[ep].partition]) {
          LOGDEBUG("Stream index:" << entry.index << " type:" << entry.type << " partition:" << entry.partition << " count:" << entry.count);
          if (entry.type == TYPE_VIDEO && _endpoints[ep].type == TYPE_VIDEO && _streams[entry.index].count > 0) {
            LOGDEBUG(" for video to :" << entry.index);
            _endpoints[ep].stream = entry.index;
            entry.endpoints.push_back(_endpoints[ep]);
            _streams[_endpoints[ep].stream].endpoints.push_back(_endpoints[ep]);
            break;
          }
          if (entry.type == TYPE_AUDIO && _endpoints[ep].type == TYPE_AUDIO && _streams[entry.index].count > 0) {
            if (entry.endpoints.size() == 0) {
              LOGDEBUG(" for audio to :" << entry.index);
              _endpoints[ep].stream = entry.index;
              entry.endpoints.push_back(_endpoints[ep]);
              _streams[_endpoints[ep].stream].endpoints.push_back(_endpoints[ep]);
              break;
            }
          }
        }
      } else {
        LOGDEBUG("endpoint->stream != 0 : " << _endpoints[ep].stream);
      }
      if (_endpoints[ep].stream > 0 && _streams[_endpoints[ep].stream].count > 0) {
        result = boost::shared_ptr<org::esb::hive::job::ProcessUnit > (new org::esb::hive::job::ProcessUnit());
        _streams[_endpoints[ep].stream].count--;

        result = _stream_queues[_endpoints[ep].stream]->dequeue();
        /*
        std::string queue_name = org::esb::util::StringUtil::toString(_endpoints[ep].stream);
        org::esb::mq::QueueMessage msg;
        _con->dequeue(queue_name, msg);
        size_t length = msg.getBufferSize();
        std::istream*buf = msg.getBufferStream();
        char * tmp = new char[length];
        buf->read(tmp, length);
        std::string data = std::string(tmp, length);
        delete []tmp;
        org::esb::io::StringInputStream sis(data);
        org::esb::io::ObjectInputStream ois(&sis);
        ois.readObject(*result.get());
         */
      }
      if (result && result->_last_process_unit) {
        LOGDEBUG("last process unit==true unit=" << result->_process_unit << " endpoint = " << ep);
        _partition_stream_map[_endpoints[ep].partition].remove(_streams[_endpoints[ep].stream]);

        foreach(std::list<Endpoint>::value_type & row, _streams[_endpoints[ep].stream].endpoints) {
          LOGDEBUG("Reset Endpoint:" << row.ep);
          _endpoints[row.ep].stream = 0;
          row.stream = 0;
        }
        _endpoints[ep].stream = 0;
      }
    } else {
      LOGDEBUG("unknown endpoint");
    }

    return result;
  }
  /*

  bool PartitionManager::getPartition(std::string name, Partition & partition) {

    foreach(const Partition & value, _partitions) {
      if (value.name == name) {
        partition = value;
        return true;
      }
    }
    return false;
  }
  bool PartitionManager::getEndpoint(boost::asio::ip::tcp::endpoint ep, PartitionManager::Endpoint & result) {

    foreach(Endpoint & value, _endpoints) {
      if (value.ep == ep) {
        result = value;
        return true;
      }
    }
    return false;
  }

  bool PartitionManager::getStream(int index, PartitionManager::Stream & result) {

    foreach(Stream & value, _streams) {
      if (value.index == index) {
        result = value;
        return true;
      }
    }
    return false;
  }
      
  bool PartitionManager::containsEndpoint(boost::asio::ip::tcp::endpoint ep) {

    foreach(Endpoint & value, _endpoints) {
      if (value.ep == ep) {
        return true;
      }
    }
    return false;
  }

  bool PartitionManager::containsStream(int index) {

    foreach(Stream & value, _streams) {
      if (value.index == index) {
        return true;
      }
    }
    return false;
  }

  bool PartitionManager::containsPartition(std::string name) {

    foreach(Partition & value, _partitions) {
      if (value.name == name) {
        return true;
      }
    }
    return false;
  }*/
}
