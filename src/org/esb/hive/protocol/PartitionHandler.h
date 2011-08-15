/* 
 * File:   PartitionHandler.h
 * Author: jhoelscher
 *
 * Created on 14. August 2011, 15:22
 */

#ifndef PARTITIONHANDLER_H
#define	PARTITIONHANDLER_H

#define JOIN_PARTITION "join partition"
#define LEAVE_PARTITION "leave partition"

#include "org/esb/hive/ProtocolCommand.h"
#include "org/esb/io/InputStream.h"
#include "org/esb/io/OutputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "boost/asio.hpp"

namespace org {
  namespace esb {
    namespace hive {

      class PartitionHandler : public org::esb::hive::ProtocolCommand {
      public:
        PartitionHandler(org::esb::io::InputStream * is, org::esb::io::OutputStream * os, boost::asio::ip::tcp::endpoint e);
        virtual ~PartitionHandler();
        int isResponsible(char * command);
        int isResponsible(cmdId & cmid);
        void process(char * command);

      private:
        org::esb::io::InputStream * _is;
        org::esb::io::OutputStream * _os;
        org::esb::io::ObjectOutputStream * _oos;
        org::esb::io::ObjectInputStream * _ois;

        boost::asio::ip::tcp::endpoint _endpoint;
        std::string _partition;
      };
    }
  }
}

#endif	/* PARTITIONHANDLER_H */

