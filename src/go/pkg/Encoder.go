package ffmpeg

//#include "libavcodec/avcodec.h"
//#include "libavcodec/opt.h"
//#include "libavformat/avformat.h"
import "C"
import "math"

type Encoder struct{
  Coder
}

func(self * Encoder)Encode(frame * Frame)*[]Packet{
  if(self.Ctx.codec_type==CODEC_TYPE_VIDEO){
    return self.encodeVideo(frame)
  }
  if(self.Ctx.codec_type==CODEC_TYPE_AUDIO){
    return self.encodeAudio(frame)
  }
  return nil
  
}

func(self * Encoder)encodeVideo(frame * Frame)*[]Packet{
  var result *[]Packet
  size := self.Ctx.width * self.Ctx.height;
  buffer_size := C.int(math.Fmax(float64(1024 * 256), float64(6 * size + 200)));
  var buffer []byte=make([]byte,int(buffer_size))
  var pbuffer * byte=&buffer[0]
  println(pbuffer)
//  ret := C.avcodec_encode_video(self.Ctx, (*C.uint8_t) (unsafe.Pointer(pbuffer)), buffer_size, (*C.AVFrame)(unsafe.Pointer(frame)));
//  println(ret)
  return result
}

func(self * Encoder)encodeAudio(frame * Frame)*[]Packet{
  var result *[]Packet
  
  return result
  
}