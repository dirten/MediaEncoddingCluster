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
#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/lang/Ptr.h"
//#include "org/esb/mq/QueueConnection.h"
#include "org/esb/util/Queue.h"
#include "Partition.h"
#include "Poco/Timestamp.h"
#include <map>
#include <list>
#include "exports.h"
namespace partitionservice {
      class PARTSERVICE_EXPORT PartitionManager {
        classlogger("partitionservice.PartitionManager")
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
        Result joinPartition(std::string name, Endpoint ep,Type=TYPE_UNKNOWN);
        Result leavePartition(std::string name, boost::asio::ip::tcp::endpoint ep);
        Result createPartition(std::string name, int size=-1);
        Result deletePartition(std::string name);
        Result clearPartition(std::string name);

        void putProcessUnit(std::string partition, boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit, Type=TYPE_UNKNOWN);
        void collectProcessUnit(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit,boost::asio::ip::tcp::endpoint ep);
        boost::shared_ptr<org::esb::hive::job::ProcessUnit>getProcessUnit(boost::asio::ip::tcp::endpoint ep);
        int getSize(std::string partition);
        int getFps();
        void resetFps();
        void addCollector( boost::function<void (boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit,boost::asio::ip::tcp::endpoint ep)> func);

      private:
        std::list<boost::function<void (boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit,boost::asio::ip::tcp::endpoint ep)> > _func_list;
        /*
        class Endpoint{
        public:
          boost::asio::ip::tcp::endpoint ep;
          Type type;
          std::string partition;
          int stream;
          bool operator==(const Endpoint&a)const;
        };
        class Stream{
        public:
          Type type;
          int index;
          std::string partition;
          std::list<Endpoint> endpoints;
          int count;
          bool operator==(const Stream&a)const;
        };*/
        /*
        class Partition{
        public:
          Partition():name(""),max_size(-1){};
          Partition(std::string name, int size=0, std::list<Endpoint> endpoints=std::list<Endpoint>()):name(name),max_size(size),endpoints(endpoints){}
          std::string name;
          int max_size;
          std::list<Endpoint> endpoints;
          bool operator==(const Partition&a)const;
        };
         */
        friend class PartitionManagerTest;
        PartitionManager();
        virtual ~PartitionManager();
        
        //std::string getPartition(Endpoint ep);
        /*
        bool containsEndpoint(boost::asio::ip::tcp::endpoint);
        bool containsStream(int);
        bool containsPartition(std::string);

        bool getEndpoint(boost::asio::ip::tcp::endpoint ep, Endpoint & result);
        bool getStream(int index, PartitionManager::Stream & result);
        bool getPartition(std::string name, PartitionManager::Partition & result);
        */
        static PartitionManager * _instance;
        typedef std::list<Stream> StreamList;
        //typedef std::map<std::string, EndpointList> PartitionEndpointMap;
        //typedef std::map<std::string, StreamList> PartitionStreamMap;
        //PartitionEndpointMap _partition_map;
        //PartitionStreamMap _partition_stream_map;

        std::map<boost::asio::ip::tcp::endpoint,Endpoint> _endpoints;
        std::map<int,Stream> _streams;
        
        //std::map<int, std::list<boost::asio::ip::tcp::endpoint> > _stream_endpoint;
        std::map<boost::asio::ip::tcp::endpoint, int > _endpoint_stream;
        std::map<int, int > _stream_max_endpoints;
        std::map<int, std::string > _partition_streams;
        std::map<std::string, int> _partition_sizes;
        std::map<int, Ptr<org::esb::util::FileQueue<boost::shared_ptr<org::esb::hive::job::ProcessUnit> > > > _stream_queues;
        //org::esb::mq::QueueConnection * _con;//(man.getUrl());

        /*new partition implmenentation*/
        typedef std::map<std::string,Partition> PartitionMap;
        typedef std::map<Endpoint, Stream> EndpointStreamMap;
        typedef std::list<boost::asio::ip::tcp::endpoint> EndpointList;
        PartitionMap _partitions;
        EndpointStreamMap _ep_stream;
        EndpointList _endpoint_pool;
        boost::mutex _partition_mutex;
        boost::mutex _collector_mutex;
        std::map<Endpoint,boost::shared_ptr<org::esb::hive::job::ProcessUnit> > _ep_pu;
        std::map<Endpoint,Partition > _ep_part;
        int _fps;
        int _pus;
        struct TimingStruct{
          int frames;
          Poco::Timestamp send;
          Poco::Timestamp recv;
        };
        typedef std::list<TimingStruct> TimingList;
        typedef std::map<Endpoint, TimingList> TimingMap;
        TimingMap _timingList;
      };
    }

#endif	/* PARTITIONMANAGER_H */

