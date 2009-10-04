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
//#include "org/esb/util/Log.h"
using namespace std;
namespace org {
    namespace esb {
        namespace io {

            class ObjectInputStream : public InputStream {
            public:
                ObjectInputStream(InputStream * is);
                long long int available(bool isBlocking = false);
                int read(unsigned char *buffer, int length);
                int read(vector < unsigned char >&buffer);
                int read();

                template < typename T > void readObject(T & object) {
                    string data;
                    _is->read(data);

                    if (!(data.length() > 0)) {
                        cout << "Fehler in der groesse INBOUND_DATA:" << data.
                                length() << endl;
                        return;
                    }
                    istringstream archive_stream(data);
                    boost::archive::binary_iarchive archive(archive_stream);
                    //boost::archive::text_iarchive archive(archive_stream);
                    archive >> object;
                }
            private:
                InputStream * _is;
            };
        }
    }
}
#endif
