/* 
 * File:   VideoFormat.cpp
 * Author: jholscher
 * 
 * Created on 29. August 2010, 16:24
 */

#include "VideoFormat.h"
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace av {

      VideoFormat::VideoFormat(std::string encoding_type) {

      }

      VideoFormat::VideoFormat(PixelFormat fmt, int width, int height, Rational framerate) {
        _pix_fmt=fmt;
        _width = width;
        _height = height;
        _framerate = framerate;
        _type=FORMAT_VIDEO;
      }

      VideoFormat::VideoFormat(AVCodecContext * ctx) {
        _width=ctx->width;
        _height=ctx->height;
        _framerate=Rational(ctx->time_base.num,ctx->time_base.den);
        _type=FORMAT_VIDEO;
      }

      VideoFormat::VideoFormat(const Format&f):Format(f){
      }
      
      void VideoFormat::setFormatOption(std::string key, std::string val){
        _options[key]=val;
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
