/* 
 * File:   Stream.h
 * Author: HoelscJ
 *
 * Created on 28. Oktober 2011, 10:05
 */

#ifndef STREAM_H
#define	STREAM_H
#include <list>

#include "Endpoint.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/lang/Ptr.h"
#include "org/esb/util/Queue.h"
using namespace org::esb::hive::job;

namespace partitionservice {
  class Partition;
  class Stream {
  public:
    enum TYPE{
      ONE_FOR_ONE,
      ONE_FOR_ALL
    };
    typedef std::list<Endpoint> EndpointList;
    
    Stream();
    Stream(std::string id, TYPE);
    virtual ~Stream();
    void enqueue(boost::shared_ptr<ProcessUnit>);
    boost::shared_ptr<ProcessUnit> dequeue();
    void clear();
    void setType(TYPE);
    TYPE getType();
    std::string getId();
    EndpointList & getEndpoints();
    void addEndpoint(Endpoint);
    //void removeEndpoint(Endpoint);
    unsigned int getMaxEndpointCount();
    void setMaxEndpointCount(unsigned int);
    int getSize();
    bool operator==(const Stream&a)const;
  private:
    friend class Partition;
    std::string _id;
    Partition *_partition;
    EndpointList _endpoints;
    TYPE _type;   
    Ptr<org::esb::util::FileQueue<boost::shared_ptr<ProcessUnit> > >_queue;
    unsigned int _max_endpoint_count;
  };
}
#endif	/* STREAM_H */

