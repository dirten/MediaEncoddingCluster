/* 
 * File:   Format.h
 * Author: jholscher
 *
 * Created on 29. August 2010, 17:40
 */

#ifndef FORMAT_H
#define	FORMAT_H
#include "AV.h"
#include "Rational.h"
namespace org {
  namespace esb {
    namespace av {

      class Format {
      public:
        
        enum FormatType{
          FORMAT_UNKNOWN,
          FORMAT_VIDEO,
          FORMAT_AUDIO
        } ;
        Format();
        Format(FormatType);
        Format(CodecID);
        FormatType getType()const ;
        void setFormatOption(std::string key, std::string val);
      protected:
        std::map<std::string, std::string> _options;
        CodecID _codec_id;
        FormatType _type;        
      };
    }
  }
}

#endif	/* FORMAT_H */

