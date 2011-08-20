/* 
 * File:   PartitionManager.h
 * Author: HoelscJ
 *
 * Created on 18. Juli 2011, 09:56
 */

#ifndef PARTITIONMANAGER_H
#define	PARTITIONMANAGER_H
#include <string>
#include <boost/asio/ip/tcp.hpp>
#include "ProcessUnit.h"
#include "org/esb/lang/Ptr.h"
#include "org/esb/mq/QueueConnection.h"
#include <map>
#include <list>
namespace org {
  namespace esb {
    namespace hive {

      class PartitionManager {
      public:

        enum Result {
          OK,
          FULL,
          EXIST,
          NOT_EXIST,
          NOT_EMPTY,
          NOT_IN_PARTITION,
          ENDPOINT_ALLREADY_JOINED
        };
        enum Type {
          TYPE_UNKNOWN,
          TYPE_VIDEO,
          TYPE_AUDIO
        };
        static PartitionManager * getInstance();
        Result joinPartition(std::string name, boost::asio::ip::tcp::endpoint ep,Type=TYPE_UNKNOWN);
        Result leavePartition(std::string name, boost::asio::ip::tcp::endpoint ep);
        Result createPartition(std::string name, int size=-1);
        Result deletePartition(std::string name);

        void putProcessUnit(std::string partition, boost::shared_ptr<job::ProcessUnit>unit, Type=TYPE_UNKNOWN);
        boost::shared_ptr<job::ProcessUnit>getProcessUnit(boost::asio::ip::tcp::endpoint ep);
      private:
        PartitionManager();
        virtual ~PartitionManager();
        std::string getPartition(boost::asio::ip::tcp::endpoint ep);
        int getStream(boost::asio::ip::tcp::endpoint ep);
        static PartitionManager * _instance;
        typedef std::list<boost::asio::ip::tcp::endpoint> EndpointList;
        typedef std::list<int> StreamList;
        typedef std::map<std::string, EndpointList> PartitionEndpointMap;
        typedef std::map<std::string, StreamList> PartitionStreamMap;
        PartitionEndpointMap _partition_map;
        PartitionStreamMap _partition_stream_map;

        //std::map<int, std::list<boost::asio::ip::tcp::endpoint> > _stream_endpoint;
        std::map<boost::asio::ip::tcp::endpoint, int > _endpoint_stream;
        std::map<int, int > _stream_max_endpoints;
        std::map<int, std::string > _partition_streams;
        std::map<std::string, int> _partition_sizes;
        org::esb::mq::QueueConnection * _con;//(man.getUrl());

      };
    }
  }
}

#endif	/* PARTITIONMANAGER_H */

