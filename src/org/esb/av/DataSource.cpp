/* 
 * File:   DataSource.cpp
 * Author: jholscher
 * 
 * Created on 29. August 2010, 14:37
 */

#include "DataSource.h"

#include "boost/thread/mutex.hpp"
namespace org {
  namespace esb {
    namespace av {

      DataSource::DataSource(MediaLocator loc) : _locator(loc) {
      }

      DataSource::~DataSource() {
        disconnect();
      }

      void DataSource::disconnect() {
        if (_isValid&&formatCtx)
          av_close_input_file(formatCtx);
        formatCtx=NULL;

      }
      void DataSource::connect() {
        //boost::mutex::scoped_lock scoped_lock(ffmpeg_mutex);
        LOGINFO("opening InputFile: " << _locator.toString());
        _isValid = false;
        AVFormatParameters params, *ap = &params;
        memset(ap, 0, sizeof (*ap));
        ap->prealloced_context = 1;



        formatCtx = avformat_alloc_context();
        std::string filename = _locator.getRemainder();
        {
          if (av_open_input_file(&formatCtx, filename.c_str(), NULL, 0, ap) != 0) {
            LOGERROR("could not open file = " << _locator.getRemainder());
            return;
          }
        }

        LOGINFO("find stream info: " << _locator.toString());
        if (av_find_stream_info(formatCtx) < 0) {
          LOGERROR("no StreamInfo from:" << _locator.toString());
          return;
        }
        if (formatCtx->iformat->flags & AVFMT_TS_DISCONT) {
          LOGDEBUG("TS DISCONT");
        }
        _isValid = true;
        LOGDEBUG("file openned: " << _locator.toString());
      }

      std::string DataSource::getContentType() {
        return "video/ffmpeg";
      }

      MediaLocator DataSource::getLocator() {
        return _locator;
      }

      void DataSource::initCheck() {
      }

      void DataSource::setLocator(MediaLocator locator) {
        _locator=locator;
      }

      void DataSource::start() {
      }

      void DataSource::stop() {
      }
    }
  }
}
