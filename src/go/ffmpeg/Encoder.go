package gmf
import "math"

type Encoder struct {
    Coder
}
func(c * Encoder)Open(){
    c.open(CODEC_TYPE_ENCODER)
}

func(c * Encoder)Encode(f Frame)*Packet{
    //println(p.avpacket)
  if(c.Ctx.ctx.codec_type==CODEC_TYPE_VIDEO){
    return c.encodeVideo(f)
  }
  if(c.Ctx.ctx.codec_type==CODEC_TYPE_AUDIO){
    return c.encodeAudio(f)
  }
  return nil
}

func(c * Encoder)encodeVideo(f Frame)*Packet{
  var result *Packet
  size := c.Ctx.ctx.width * c.Ctx.ctx.height;
  buffer_size := int(math.Fmax(float64(1024 * 256), float64(6 * size + 200)));
  var buffer []byte=make([]byte,int(buffer_size))
  //var pbuffer * byte=&buffer[0]
  //println(pbuffer)
  avcodec_encode_video(&c.Ctx, &buffer, &buffer_size, &f);
  return result
 
    return nil
}

func(c * Encoder)encodeAudio(f Frame)*Packet{
    return nil
}
