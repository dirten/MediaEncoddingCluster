/* 
 * File:   CCodec.cpp
 * Author: HoelscJ
 * 
 * Created on 30. August 2010, 13:12
 */

#include "CCodec.h"
namespace org {
  namespace esb {
    namespace av {

      CCodec::CCodec() {
      }

      CCodec::CCodec(const CCodec& orig) {
      }

      CCodec::~CCodec() {
      }

      void CCodec::open() {
      }

      void CCodec::close() {
      }

      std::string CCodec::getName() {
      }

      void CCodec::reset() {
      }

      std::list<Format> CCodec::getSupportedInputFormats() {
      }

      std::list<Format> CCodec::getSupportedOutputFormats(Format in ) {
      }

      int CCodec::process(Buffer & in, Buffer & out) {
      }

      Format CCodec::setInputFormat(Format f) {
        f.getType();
      }

      Format CCodec::setOutputFormat(Format f) {
        f.getType();
      }
    }
  }
}
