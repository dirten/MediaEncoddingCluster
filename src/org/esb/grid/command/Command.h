/* 
 * File:   Command.h
 * Author: HoelscJ
 *
 * Created on 31. Dezember 2010, 12:58
 */

#ifndef COMMAND_H
#define	COMMAND_H
#include <string>
//#include <boost/serialization/binary_object.hpp>
#include <boost/archive/basic_archive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>


namespace org{
  namespace esb{
    namespace grid{
      struct CommandHeader{
        int messageType;
        int messageSize;
      };
      class Command{
      public:
        CommandHeader header;
        std::string operation;
        std::string operationdata;
      private:
        friend class boost::serialization::access;

        template < class Archive >
        void serialize(Archive & ar, const unsigned int version){
          ar & header.messageType;
          ar & header.messageSize;
          ar & operation;
          ar & operationdata;
        }
      };
    }
  }
}

#endif	/* COMMAND_H */

