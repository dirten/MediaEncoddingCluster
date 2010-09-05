#include "FormatOutputStream.h"
#include "AV.h"

using namespace org::esb::av;
using namespace org::esb::io;

FormatOutputStream::FormatOutputStream(File * target_file, const char * fmt) {
  _status = CLOSED;

  AVFormatParameters params, *ap = &params;
  memset(ap, 0, sizeof (*ap));


  _file = target_file;
  _fmtCtx = avformat_alloc_context();
  _fmt = guess_format(fmt, target_file->getPath().c_str(), NULL);
  _fmtCtx->oformat = _fmt;

  if (!(_fmt->flags & AVFMT_NOFILE)) {
    if (url_fopen(&_fmtCtx->pb, _file->getPath().c_str(), URL_WRONLY) < 0) {
      fprintf(stderr, "Could not open '%s'\n", _file->getPath().c_str());
      //            exit(1);
    }
  }

  if (av_set_parameters(_fmtCtx, ap) < 0) {
    fprintf(stderr, "Invalid output format parameters\n");
    exit(1);
  }
  _fmtCtx->preload=static_cast<int>(0.5*AV_TIME_BASE);
  _fmtCtx->loop_output=AVFMT_NOOUTPUTLOOP;
  _fmtCtx->flags|=AVFMT_FLAG_NONBLOCK;
//  logdebug("PRELOAD:"<<0.5*AV_TIME_BASE);
  _status = OPENED;

}

FormatOutputStream::~FormatOutputStream() {
  close();
}
 void FormatOutputStream::dumpFormat() {
        dump_format(_fmtCtx, 0, _file->getPath().c_str(), 1);
      }
void FormatOutputStream::addPacketStream(PacketOutputStream & pos, Encoder & encoder) {


}

void FormatOutputStream::write(char * buffer, int length) {
};

void FormatOutputStream::write(vector<unsigned char>&buffer) {
};

void FormatOutputStream::write(char * buffer) {
};

void FormatOutputStream::open() {
/*  if (!(_fmt->flags & AVFMT_NOFILE)) {
    if (url_fopen(&_fmtCtx->pb, _file->getPath().c_str(), URL_WRONLY) < 0) {
      fprintf(stderr, "Could not open '%s'\n", _file->getPath().c_str());
      //            exit(1);
    }
  }*/
}

void FormatOutputStream::close() {
  if (!(_fmt->flags & AVFMT_NOFILE)&&_status == OPENED) {
    url_fclose(_fmtCtx->pb);
    int nb_streams = _fmtCtx->nb_streams;
    for (int a = 0; a < nb_streams; a++) {
      av_free(_fmtCtx->streams[a]);
    }
    av_free(_fmtCtx);
    _status = CLOSED;
  }
  //	av_close_input_file(_fmtCtx);
};
