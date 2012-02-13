/* 
 * File:   UUID.cpp
 * Author: HoelscJ
 * 
 * Created on 26. Oktober 2011, 17:02
 */

#include "UUID.h"
//#include "Poco/UUID.h"
//#include "Poco/UUIDGenerator.h"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "boost/lexical_cast.hpp"

namespace org {
  namespace esb {
    namespace util {

      PUUID::PUUID() {
        //Poco::UUIDGenerator& generator = Poco::UUIDGenerator::defaultGenerator();
        //Poco::UUID uuid(generator.create());        
        //_uuid = uuid.toString();//boost::lexical_cast<std::string > (uuid);
        boost::uuids::uuid uuid = boost::uuids::random_generator()();
        _uuid = boost::lexical_cast<std::string > (uuid);
      }

      PUUID::~PUUID() {
      }
    }
  }
}
