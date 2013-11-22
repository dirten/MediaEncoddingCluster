#ifndef SERIALIZING_H
#define SERIALIZING_H
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
namespace org {
  namespace esb {
    namespace util {
      class Serializing
      {
        private:
          Serializing();

        public:
          template<typename T>
          static std::string serialize(const T &object) {
              std::ostringstream archive_stream;
              boost::archive::text_oarchive archive(archive_stream);
              archive << object;
              std::string _outbound_data = archive_stream.str();
              return _outbound_data;
          }

          template < typename T >
          static int deserialize(T & object, std::string data) {
            if (data.length() <= 0) {
              return -1;
            }
            std::istringstream archive_stream(data);
            boost::archive::text_iarchive archive(archive_stream);
            try {
              archive >> object;
            } catch (std::exception & ex) {
              throw ex;
              //return -1;
            }
            return 0;
          }
      };
    }
  }
}
#endif // SERIALIZING_H
