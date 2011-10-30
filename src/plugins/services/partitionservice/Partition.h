/* 
 * File:   Partition.h
 * Author: HoelscJ
 *
 * Created on 28. Oktober 2011, 10:04
 */

#ifndef PARTITION_H
#define	PARTITION_H
#include "Stream.h"
#include <list>
namespace partitionservice {

  class Partition {
  public:
    typedef std::list<Stream> StreamList;
    typedef std::list<Endpoint> EndpointList;

    Partition();
    Partition(std::string id);
    virtual ~Partition();
    void addStream(Stream );
    bool containStream(std::string id);
    Stream & getStream(std::string id);
    StreamList getStreams();
    std::string getId();
    
    void addEndpoint(Endpoint );
    
    EndpointList getEndpoints();
    void removeEndpoint(Endpoint );
    void removeStream(Stream );
  private:
    StreamList _streams;
    EndpointList _endpoints;
    std::string _id;
  };
}

#endif	/* PARTITION_H */

