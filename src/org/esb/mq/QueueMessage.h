/* 
 * File:   QueueMessage.h
 * Author: HoelscJ
 *
 * Created on 17. Dezember 2010, 11:55
 */

#ifndef QUEUEMESSAGE_H
#define	QUEUEMESSAGE_H
#include <inttypes.h>
#include "safmq.h"
#include <sstream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace mq {

      class QueueMessage:public safmq::QueueMessage{
      public:
        QueueMessage();
        template<typename T>
        void setObject(const T &object) {
            std::ostringstream archive_stream;
            boost::archive::binary_oarchive archive(archive_stream);
            archive << object;
            std::string _outbound_data = archive_stream.str();
            this->getBufferStream()->write(_outbound_data.c_str(),_outbound_data.length());
        }
        template < typename T >
        int getObject(T & object) {
          //string data;
          size_t size=this->getBufferSize();
          char * d=new char[size];
            std::istream * buf=this->getBufferStream();
            buf->read(d, size);
            std::string data(d, size);
            delete []d;
          if (!(data.length() > 0)) {
            LOGERROR("Fehler in der groesse INBOUND_DATA:" << data.length());
            return -1;
          }
          std::istringstream archive_stream(data);
          boost::archive::binary_iarchive archive(archive_stream);
          //boost::archive::text_iarchive archive(archive_stream);
          try {
            archive >> object;
          } catch (std::exception & ex) {
            LOGERROR("Exception reading archive:"<<ex.what());
            return -1;
          }
          return 0;

        }

        virtual ~QueueMessage();
      private:

      };
    }
  }
}

#endif	/* QUEUEMESSAGE_H */

