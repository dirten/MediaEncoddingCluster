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
#include "org/esb/config/config.h"
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
    _fps=0;
    _pus=0;
  }

  PartitionManager::~PartitionManager() {
  }

  PartitionManager::Result PartitionManager::joinPartition(std::string name, Endpoint ep, Type type) {
    PartitionManager::Result result = PartitionManager::OK;
    if (!_partitions.count(name) > 0) {
      return PartitionManager::NOT_EXIST;

    }

    foreach(PartitionMap::value_type partition, _partitions) {

      LOGDEBUG("Partition")
      foreach(Endpoint end, partition.second.getEndpoints()) {
        if (end == ep)
          return PartitionManager::ENDPOINT_ALLREADY_JOINED;

      }
    }
    if (_partitions.count(name) > 0) {
      _partitions[name].addEndpoint(Endpoint(ep));
    } else {
      result = PartitionManager::NOT_EXIST;
    }
    return result;
  }

  /**
   * @param partition
   * @return 
   */
  int PartitionManager::getSize(std::string partition) {
    int result = _ep_pu.size();
    LOGDEBUG("_ep_pu size "<<partition<<"="<<result);
    if (_partitions.count(partition) > 0) {

      foreach(Stream s, _partitions[partition].getStreams()) {
        result += s.getSize();
      }
    }
    LOGDEBUG("partition size "<<partition<<"="<<result);
    return result;
  }

  PartitionManager::Result PartitionManager::leavePartition(std::string name, boost::asio::ip::tcp::endpoint ep) {
    PartitionManager::Result result = PartitionManager::NOT_IN_PARTITION;
    LOGDEBUG("PartitionManager::leavePartition(std::string name, boost::asio::ip::tcp::endpoint ep) name="<<name<<" ep="<<ep);
    /* @TODO: when client crashes, put back the ProcessUnit to the queue
     * that should be handled by an other Client
     */

    _ep_stream.erase(ep);
    
    if(_ep_pu.count(ep)){
      Type t;
      if (_ep_pu[ep]->_decoder->getCodecType() == AVMEDIA_TYPE_AUDIO) {
        t = partitionservice::PartitionManager::TYPE_AUDIO;
      } else if (_ep_pu[ep]->_decoder->getCodecType() == AVMEDIA_TYPE_VIDEO) {
        t = partitionservice::PartitionManager::TYPE_VIDEO;
      }
      putProcessUnit(name.length()==0?"global":name,_ep_pu[ep],t);
      _ep_pu.erase(ep);
    }
    foreach(PartitionMap::value_type & partition, _partitions) {

      foreach(Endpoint end, partition.second.getEndpoints()) {
        if (end == ep) {
          LOGDEBUG("partition.second.removeEndpoint(end);");
          partition.second.removeEndpoint(end);
          result = PartitionManager::OK;
          return result;

        }
      }
      return result;
      foreach(Stream str, partition.second.getStreams()) {
        Stream & s = partition.second.getStream(str.getId());
        
        foreach(Endpoint end, s.getEndpoints()) {
          LOGDEBUG("Endpoint" << ep << " from stream" << str.getId());
          if (end == ep) {
            LOGDEBUG("Size:" << s.getEndpoints().size());
            s.getEndpoints().remove(end);
            LOGDEBUG("Size:" << s.getEndpoints().size());
            partition.second.removeEndpoint(end);
            result = PartitionManager::OK;

            LOGDEBUG("Endpoint" << ep << " from stream" << str.getId() << " removed");
          }
        }
      }
    }
    return result;
  }

  PartitionManager::Result PartitionManager::createPartition(std::string name, int size) {
    PartitionManager::Result result = PartitionManager::OK;
    if (_partitions.count(name) == 0) {
      //Partition part(name);
      _partitions[name] = Partition(name); //part;
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
    //if (unit->_input_packets.size() == 0)return;

    //_input_packets.front()->getStreamIndex();
    std::string stream_index = org::esb::util::StringUtil::toString(unit->_source_stream);
    std::string stream_id = org::esb::config::Config::get("hive.tmp_path") + "/jobs/" + unit->getJobId() + "/" + stream_index;

    if (_partitions.count(partition) > 0) {
      Partition & part = _partitions[partition];
      if (!part.containStream(stream_id)) {
        Stream::TYPE stype = type == TYPE_AUDIO ? Stream::ONE_FOR_ALL : Stream::ONE_FOR_ONE;
        part.addStream(Stream(stream_id, stype));
      }
      Stream & stream = part.getStream(stream_id);
      //Ptr<org::esb::hive::job::ProcessUnit>u(unit);
      stream.enqueue(unit);
      LOGDEBUG("Stream size="<<stream.getSize());
    }
  }

  boost::shared_ptr<org::esb::hive::job::ProcessUnit>PartitionManager::getProcessUnit(boost::asio::ip::tcp::endpoint ep) {
    LOGDEBUG("Enter PartitionManager::getProcessUnit");
    boost::mutex::scoped_lock partition_get_lock(_partition_mutex);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> result;
    LOGDEBUG("getProcessUnit Endpoint:" << ep);
    /*when the endpoint is associated to a stream*/
    if (_ep_stream.count(ep)) {
      LOGDEBUG("Endpoint:" << ep << " Stream:" << _ep_stream[ep].getId() << " Size:" << _ep_stream[ep].getSize());
      result = _ep_stream[ep].dequeue();
    } else {

      /*search for the next partition that have streams with no endpoints*/
      foreach(PartitionMap::value_type & partition, _partitions) {
        LOGDEBUG("look into partition:" << partition.first);

        foreach(Stream s, partition.second.getStreams()) {
          LOGDEBUG("look into stream:" << s.getId());
          Stream & str = partition.second.getStream(s.getId());
          if (str.getEndpoints().size() == 0) {
            LOGDEBUG("adding endpoint" << ep << " to stream:" << str.getId());
            str.addEndpoint(ep);
            result = str.dequeue();
            _ep_stream[ep] = str;
            break;
          } else {
            LOGDEBUG("EndpointSize=" << str.getEndpoints().size());
          }
        }
      }
    }


    /*when result will be also empty then it is a spare endpoint*/
    /*this will be associated to next free Stream which have not reached his max Endpoints*/
    if (!result) {

      /*search for the next partition that have streams with no endpoints*/
      foreach(PartitionMap::value_type & partition, _partitions) {
        LOGDEBUG("look into partition:" << partition.first);

        foreach(Stream s, partition.second.getStreams()) {
          LOGDEBUG("look into stream:" << s.getId());
          Stream & str = partition.second.getStream(s.getId());
          if (str.getType() == Stream::ONE_FOR_ONE && str.getEndpoints().size() < str.getMaxEndpointCount()) {
            LOGDEBUG("adding spare endpoint to stream:" << str.getId());
            str.addEndpoint(ep);
            result = str.dequeue();
            _ep_stream[ep] = str;
            break;
          }
        }
      }
    }
    /*when the last ProcessUnit will be picked up then delete the association between 
     Endpoint and Stream and finaly delete the stream out of the partition*/
    if (result && result->_last_process_unit) {
      LOGDEBUG("last process unit==true unit=" << result->_process_unit << " endpoint = " << ep);

      foreach(PartitionMap::value_type & partition, _partitions) {

        foreach(Stream s, partition.second.getStreams()) {
          Stream & str = partition.second.getStream(s.getId());

          foreach(Endpoint end, str.getEndpoints()) {
            if (end == ep) {

              foreach(Endpoint endin, str.getEndpoints()) {
                _ep_stream.erase(endin);
                LOGDEBUG("Stream EndpointSize before:" << str.getEndpoints().size());
                //str.getEndpoints().remove(endin);
                LOGDEBUG("Stream EndpointSize after:" << str.getEndpoints().size());
                LOGDEBUG("Remove endpoint:" << ep << " Stream:" << str.getId());

                //LOGDEBUG("Partition EndpointSize before:"<<partition.second.getEndpoints());
              }
              str.getEndpoints().clear();
              partition.second.removeStream(str);
              //_ep_stream.erase(ep);
              break;
            }
          }
        }
      }
    }
    if(result){
      if(_ep_pu.count(ep)){
        LOGERROR("Endpoint is allready Processing a ProcessUnit");
      }
      _ep_pu[ep]=result;
    }
    LOGDEBUG("Leave PartitionManager::getProcessUnit");
    return result;
  }

  void PartitionManager::collectProcessUnit(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit,boost::asio::ip::tcp::endpoint ep) {

    std::string name = org::esb::config::Config::get("hive.tmp_path") +"/jobs/"+unit->getJobId()+"/collect";
    name += "/";

    org::esb::io::File outdir(name.c_str());
    if(!outdir.exists())
      outdir.mkdirs();
    std::string uuid = org::esb::util::PUUID();
    name += uuid;
    org::esb::io::File out(name.c_str());

    org::esb::io::FileOutputStream fos(&out);
    org::esb::io::ObjectOutputStream ous(&fos);

    ous.writeObject(unit);
    ous.close();
    
    if(_ep_pu.count(ep)){
      if(unit->getFps()>0){
        //std::cerr <<"hallo fps="<<_fps<<" unit.fps="<<unit->getFps()<<" pus="<<_pus<<std::endl;
        _pus++;
        _fps+=unit->getFps();
      }
      _ep_pu.erase(ep);
    }else{
      LOGERROR("Endpoint have not previously getting a ProcessUnit");
    }
  }
  
  int PartitionManager::getFps(){
    int fps=0;
    if(_fps>0&&_pus>0)
      fps=_fps/_pus;
    return fps;
  }

  void PartitionManager::resetFps(){
    _fps=0;
    _pus=0;
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
