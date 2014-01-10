#ifndef SERIALIZING_H
#define SERIALIZING_H
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "org/esb/io/portable_binary_oarchive.hpp"
#include "org/esb/io/portable_binary_iarchive.hpp"
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
              portable_binary_oarchive archive(archive_stream);
              //boost::archive::text_oarchive archive(archive_stream);
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
            portable_binary_iarchive archive(archive_stream);
            //boost::archive::text_iarchive archive(archive_stream);
            try {
              archive >> object;
            } catch (boost::archive::archive_exception & ex) {
              std::cout << "Exception=" << ex.what() <<" code="<<ex.code<<std::endl;
              throw ex;
              //return -1;
            }
            return 0;
          }

          template < typename T >
          static int deserialize(T & object, std::istream & data) {
            portable_binary_iarchive archive(data);
            //boost::archive::text_iarchive archive(data);
            try {
              archive >> object;
            } catch (boost::archive::archive_exception & ex) {
              std::cout << "Exception=" << ex.what() <<" code="<<ex.code<<std::endl;
              throw ex;
            }
            return 0;
          }


          template<typename T>
          static void serialize(const T &object, std::ostream & ost) {
              portable_binary_oarchive archive(ost);
              //boost::archive::text_oarchive archive(ost);
              archive << object;
          }

      };
    }
  }
}
#endif // SERIALIZING_H
