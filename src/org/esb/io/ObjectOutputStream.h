#ifndef ORG_ESB_IO_OBJECTOUTPUTSTREAM_H
#define ORG_ESB_IO_OBJECTOUTPUTSTREAM_H
#include "OutputStream.h"
#include "ObjectStream.h"
//#include <boost/lexical_cast.hpp>
//#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
//#include <boost/iostreams/device/array.hpp>
#include <fstream>
#include "org/esb/io/exports.h"
#include "org/esb/util/Serializing.h"

using org::esb::util::Serializing;

namespace org {
  namespace esb {
    namespace io {

      class  IO_EXPORT ObjectOutputStream : public OutputStream {
        public:
          ObjectOutputStream(OutputStream * out);

          void write(char buffer);
          void write(char * buffer, int length);
          void write(vector <unsigned char >& buffer);
          bool close();
          void flush();

          template<typename T>
          void writeObject(const T &object) {
            std::string data=Serializing::serialize(object);
            _os->write((char*) data.c_str(), data.length());
            _os->flush();
          }

        private:
          OutputStream * _os;
      };
    }
  }
}
#endif
