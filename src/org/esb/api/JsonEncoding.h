/* 
 * File:   JsonEncoding.h
 * Author: HoelscJ
 *
 * Created on 18. Mai 2011, 14:46
 */

#ifndef JSONENCODING_H
#define	JSONENCODING_H
#include "org/esb/db/hivedb.hpp"
#include "libjson/libjson.h"

namespace org {
  namespace esb {
    namespace api {

      class JsonEncoding:public JSONNode {
      public:
        JsonEncoding(std::string);
        JsonEncoding(db::Job);
        JsonEncoding(const JsonEncoding& orig);
        virtual ~JsonEncoding();
      private:

      };
    }
  }
}

#endif	/* JSONENCODING_H */

