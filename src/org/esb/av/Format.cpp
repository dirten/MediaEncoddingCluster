/* 
 * File:   Format.cpp
 * Author: jholscher
 * 
 * Created on 29. August 2010, 17:40
 */

#include "Format.h"
namespace org {
  namespace esb {
    namespace av {
      Format::Format(){
        _type=Format::FORMAT_UNKNOWN;
      }
      
      Format::FormatType Format::getType()const{
        return _type;
      }

    }
  }
}
