/* 
 * File:   CCodec.h
 * Author: HoelscJ
 *
 * Created on 30. August 2010, 13:12
 */

#ifndef CCODEC_H
#define	CCODEC_H
#include "PlugIn.h"
#include "Format.h"
#include <list>
#include "Buffer.h"
namespace org {
  namespace esb {
    namespace av {

      class CCodec:public PlugIn {
      public:
        CCodec();
        CCodec(const CCodec& orig);
        virtual ~CCodec();
        /*functions from Plugin*/
        void open();
        void close();
        std::string getName();
        void reset();
        /*functions for codec*/
        std::list<Format> getSupportedInputFormats();
        std::list<Format> getSupportedOutputFormats(Format in=Format());
        int process(Buffer & in, Buffer & out);
        Format setInputFormat(Format f);
        Format setOutputFormat(Format f);

      private:

      };
    }
  }
}

#endif	/* CCODEC_H */

