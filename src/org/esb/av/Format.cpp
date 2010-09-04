/* 
 * File:   Format.cpp
 * Author: jholscher
 * 
 * Created on 29. August 2010, 17:40
 */

#include "Format.h"
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace av {

      Format::Format(){
        _type=Format::FORMAT_UNKNOWN;
      }

      Format::Format(FormatType type):
        _codec_id(CODEC_ID_NONE),
        _type(type){
        _type=type;
      }

      Format::Format(CodecID id):
        _codec_id(id),
        _type(Format::FORMAT_UNKNOWN){
        _codec_id=id;
      }

      void Format::setFormatOption(std::string key, std::string val){
   
      }
      
      Format::FormatType Format::getType()const{
        return _type;
      }

    }
  }
}
