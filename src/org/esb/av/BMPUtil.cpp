#include "BMPUtil.h"
#include "Frame.h"
#include "FormatOutputStream.h"
#include "PacketOutputStream.h"
#include "Encoder.h"
#include "org/esb/io/File.h"
namespace org {
  namespace esb {
    namespace av {
       Frame * BMPUtil::load(std::string file){
         return NULL;
      }
       void BMPUtil::save(std::string file, Frame * frame){
        File f(file);
        FormatOutputStream fos(&f);
        fos._fmtCtx->ctx_flags|=AVFMT_NOFILE;
        
        memcpy(&fos._fmtCtx->filename,file.c_str(),file.length());
        PacketOutputStream pos(&fos);
        Encoder enc(CODEC_ID_BMP);
        enc.setTimeBase(1,1);
        enc.setWidth(frame->getWidth());
        enc.setHeight(frame->getHeight());
        enc.setPixelFormat(PIX_FMT_BGR24);
        enc.open();
        LOGDEBUG(enc.toString());
        pos.setEncoder(enc,0);
        pos.init();
        enc.setOutputStream(&pos);
        enc.encode(*frame);
      }
    }
  }
}