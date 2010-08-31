/* 
 * File:   Demultiplexer.cpp
 * Author: jholscher
 * 
 * Created on 29. August 2010, 20:01
 */

#include "Demultiplexer.h"
namespace org {
  namespace esb {
    namespace av {

      Demultiplexer::Demultiplexer(DataSource& s) : _source(s) {
        for (int a = 0; a < _source.formatCtx->nb_streams; a++) {
          Ptr<Track> track = new Track(_source.formatCtx->streams[a], this);
          _track_list.push_back(track);
        }
        _end_of_media=!readFrameFromContext();
      }

      Duration Demultiplexer::getDuration() {
        return Duration(_source.formatCtx->duration);
      }

      TimeStamp Demultiplexer::getMediaTime() {
        return TimeStamp(_source.formatCtx->timestamp);
      }

      std::list<Ptr<Track> > Demultiplexer::getTracks() {
        return _track_list;
      }

      bool Demultiplexer::isPositionable() {
        return true;
      }

      bool Demultiplexer::isRandomAccess() {
        return true;
      }

      TimeStamp Demultiplexer::setPosition(TimeStamp t, int r) {
        return av_seek_frame(_source.formatCtx, -1, t.getTime(), r);
      }

      void Demultiplexer::readFrame(Buffer &buf, int stream_index) {
        boost::mutex::scoped_lock next_packet_wait_lock(_next_packet_wait_mutex);
        while (_next_packet->stream_index != stream_index&&!_end_of_media) {
          _next_packet_wait_condition.wait(next_packet_wait_lock);
        }
        buf.setAVPacket(_next_packet);
        buf.setEOM(_end_of_media);
        _end_of_media=!readFrameFromContext();
        _next_packet_wait_condition.notify_all();
      }

      bool Demultiplexer::readFrameFromContext() {
        bool result=true;
        boost::mutex::scoped_lock scoped_lock(_read_frame_mutex);
        _next_packet = new AVPacket();
        av_init_packet(_next_packet.get());
        if (av_read_frame(_source.formatCtx, _next_packet.get()) >= 0) {
          av_dup_packet(_next_packet.get());
        } else {
          result=false;
        }
        return result;
      }
      Demultiplexer::~Demultiplexer() {
      }
    }
  }
}
