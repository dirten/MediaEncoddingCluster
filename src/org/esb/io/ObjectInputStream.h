#ifndef ORG_ESB_IO_OBJECTINPUTSTREAM
#define ORG_ESB_IO_OBJECTINPUTSTREAM
//#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <fstream>
#include <iostream>
//#include <ostream>
//#include <sstream>
#include "ObjectStream.h"
#include "InputStream.h"
#include "org/esb/util/Log.h"
using namespace std;
namespace org {
  namespace esb {
    namespace io {

      class ObjectInputStream : public InputStream {
        classlogger("org.esb.io.ObjectInputStream");
      public:
        ObjectInputStream(InputStream * is);
        long long int available(bool isBlocking = false);
        int read(unsigned char *buffer, int length);
        int read(vector < unsigned char >&buffer);
        int read();

        template < typename T > int readObject(T & object) {
          string data;
          _is->read(data);

          if (!(data.length() > 0)) {
            LOGERROR("Fehler in der groesse INBOUND_DATA:" << data.length());
            return -1;
          }
          istringstream archive_stream(data);
          boost::archive::binary_iarchive archive(archive_stream);
          //boost::archive::text_iarchive archive(archive_stream);
          try {
            archive >> object;
          } catch (exception & ex) {
            LOGERROR("Exception reading archive:"<<ex.what());
            return -1;
          }
          return 0;

        }
      private:
        InputStream * _is;
      };
    }
  }
}
#endif
