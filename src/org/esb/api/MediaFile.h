/* 
 * File:   MediaFile.h
 * Author: HoelscJ
 *
 * Created on 4. April 2011, 16:55
 */

#ifndef MEDIAFILE_H
#define	MEDIAFILE_H
#include <string>
#include "org/esb/rpc/rpc.pb.h"


namespace org {
  namespace esb {
    namespace api {

      class MediaFile : public org::esb::rpc::Mediafile{
      public:
        MediaFile();
        virtual ~MediaFile();
      private:
      };
    }
  }
}

#endif	/* MEDIAFILE_H */

