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
using namespace org::esb::hive::job;

namespace partitionservice {

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
    void enqueue(Ptr<ProcessUnit>);
    void setType(TYPE);
    TYPE getType();
    std::string getId();
    EndpointList getEndpoints();
    
    bool operator==(const Stream&a)const;
  private:
    std::string _id;
    EndpointList _endpoints;
    TYPE _type;    
  };
}
#endif	/* STREAM_H */

