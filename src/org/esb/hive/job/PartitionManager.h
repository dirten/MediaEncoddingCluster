/* 
 * File:   PartitionManager.h
 * Author: HoelscJ
 *
 * Created on 18. Juli 2011, 09:56
 */

#ifndef PARTITIONMANAGER_H
#define	PARTITIONMANAGER_H
#include <string>
#include <boost/asio.hpp>
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
          ALLREADY_JOINED
        };
        static PartitionManager * getInstance();
        Result joinPartition(std::string name, boost::asio::ip::tcp::endpoint ep);
        Result leavePartition(std::string name, boost::asio::ip::tcp::endpoint ep);
        Result createPartition(std::string name, int size);
        Result deletePartition(std::string name);

        void putProcessUnit(std::string partition, boost::shared_ptr<job::ProcessUnit>unit);
        //boost::shared_ptr<job::ProcessUnit>getProcessUnit(std::string partition,, boost::asio::ip::tcp::endpoint ep);
        int getStream(boost::asio::ip::tcp::endpoint ep);
      private:
        PartitionManager();
        virtual ~PartitionManager();
        static PartitionManager * _instance;
        
        std::map<std::string, std::list<boost::asio::ip::tcp::endpoint> > _partition_map;
        std::map<int, std::list<boost::asio::ip::tcp::endpoint> > _stream_endpoint;
        std::map<int, int > _stream_max_endpoints;
        std::map<int, std::string > _partition_streams;
        std::map<std::string, int> _partition_sizes;
        org::esb::mq::QueueConnection * _con;//(man.getUrl());

      };
    }
  }
}

#endif	/* PARTITIONMANAGER_H */

