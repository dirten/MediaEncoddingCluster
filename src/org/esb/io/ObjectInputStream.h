#ifndef ORG_ESB_IO_OBJECTINPUTSTREAM
#define ORG_ESB_IO_OBJECTINPUTSTREAM
//#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <fstream>
#include <iostream>
//#include <ostream>
#include <sstream>
#include "ObjectStream.h"
#include "InputStream.h"
//#include "org/esb/util/Log.h"
#include "org/esb/io/exports.h"
#include "org/esb/util/Serializing.h"
using namespace std;
using org::esb::util::Serializing;
namespace org {
  namespace esb {
    namespace io {

      class IO_EXPORT ObjectInputStream : public InputStream {
        //classlogger("org.esb.io.ObjectInputStream");
      public:
        ObjectInputStream(InputStream * is);
        long long int available(bool isBlocking = false);
        int read(unsigned char *buffer, int length);
        int read(vector < unsigned char >&buffer);
        int read();

        template < typename T > int readObject(T & object) {
            string data;
            _is->read(data);
            Serializing::deserialize(object, data);
            return data.size();
        }
      private:
        InputStream * _is;
      };
    }
  }
}
#endif
