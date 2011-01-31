package gmf
import "math"
import "unsafe"
type Encoder struct {
    Coder
}
func(c * Encoder)Open(){
    c.open(CODEC_TYPE_ENCODER)
}

func(c * Encoder)Encode(f Frame)*Packet{
  if(c.Ctx.ctx.codec_type==CODEC_TYPE_VIDEO){
    return c.encodeVideo(f)
  }
  if(c.Ctx.ctx.codec_type==CODEC_TYPE_AUDIO){
    return c.encodeAudio(f)
  }
  return nil
}

func(c * Encoder)encodeVideo(f Frame)*Packet{
  var result *Packet=new(Packet)
  av_init_packet(result)
  size := c.Ctx.ctx.width * c.Ctx.ctx.height;
  buffer_size := int(math.Fmax(float64(1024 * 256), float64(6 * size + 200)));
  var buffer =av_malloc(buffer_size)//byte=make([]byte,buffer_size)
  esize:=avcodec_encode_video(&c.Ctx, buffer, &buffer_size, &f);
  
  if(esize>0){
    result.avpacket.size=(_Ctype_int)(esize)
    result.avpacket.data=(*_Ctypedef_uint8_t)(unsafe.Pointer(&buffer))
    result.avpacket.stream_index=0
    result.avpacket.duration=0
    result.avpacket.flags=0
    return result
  }
return nil 
}

func(c * Encoder)encodeAudio(f Frame)*Packet{
    return nil
}
