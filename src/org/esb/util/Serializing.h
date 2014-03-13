#ifndef SERIALIZING_H
#define SERIALIZING_H
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "org/esb/io/portable_binary_oarchive.hpp"
#include "org/esb/io/portable_binary_iarchive.hpp"
//#include "org/esb/io/portable_iarchive.hpp"
//#include "org/esb/io/portable_oarchive.hpp"

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
              serialize<boost::archive::text_oarchive>(object, archive_stream);
              return archive_stream.str();
          }

          template < typename T >
          static void deserialize(T & object, std::string data) {
            std::istringstream archive_stream(data);
            deserialize<boost::archive::text_iarchive>(object,archive_stream);
          }

          template <class C, typename T>
          static int deserialize(T & object, std::istream & data) {
            C archive(data);
            try {
              archive >> object;
            } catch (boost::archive::archive_exception & ex) {
              std::cout << "Exception=" << ex.what() <<" code="<<ex.code<<std::endl;
              throw ex;
            }
            return 0;
          }

          template<class C,typename T>
          static void serialize(const T &object, std::ostream & ost) {
              C archive(ost);
              archive << object;
          }
      };
    }
  }
}
#endif // SERIALIZING_H
