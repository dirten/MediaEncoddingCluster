/* 
 * File:   VideoFormat.cpp
 * Author: jholscher
 * 
 * Created on 29. August 2010, 16:24
 */

#include "VideoFormat.h"
namespace org {
  namespace esb {
    namespace av {

      VideoFormat::VideoFormat(PixelFormat pxfm, int width, int height, Rational framerate) {
        _pix_fmt = pxfm;
        _width = width;
        _height = height;
        _framerate = framerate;
        _type=FORMAT_VIDEO;
      }

      VideoFormat::VideoFormat(const Format&f){
        _pix_fmt=f._pix_fmt;
        _width=f._width;
        _height=f._height;
        _framerate=f._framerate;
        _type=FORMAT_VIDEO;
      }

      PixelFormat VideoFormat::getPixelFormat(){
        return _pix_fmt;
      }

      int VideoFormat::getWidth(){
        return _width;
      }

      int VideoFormat::getHeight(){
        return _height;
      }

      bool VideoFormat::operator==(VideoFormat t) {
        return t._pix_fmt==_pix_fmt&&
                t._width==_width&&
                t._height==_height&&
                t._framerate==_framerate;
      }

      std::string VideoFormat::toString(){
        std::string result;
        return result;
      }
    }
  }
}
