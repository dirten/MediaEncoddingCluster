package gmf
import "math"
import "unsafe"
type Encoder struct {
    Coder
    audio_fifo * AVFifoBuffer
}

func(c * Encoder)Open(){
    c.open(CODEC_TYPE_ENCODER)
}

func(c * Encoder)Encode(f * Frame)*Packet{
  if(c.Ctx.ctx.codec_type==CODEC_TYPE_VIDEO){
    return c.encodeVideo(f)
  }
  if(c.Ctx.ctx.codec_type==CODEC_TYPE_AUDIO){
    return c.encodeAudio(f)
  }
  return nil
}

func(c * Encoder)encodeVideo(f * Frame)*Packet{
  var result *Packet=new(Packet)
  av_init_packet(result)
  size := c.Ctx.ctx.width * c.Ctx.ctx.height;
  buffer_size := int(math.Fmax(float64(1024 * 256), float64(6 * size + 200)));
  //println("buffer size")
  //println(buffer_size)
  //var buffer =new ([]byte)
  buffer/*[]byte*/ :=make([]byte,buffer_size)
  esize:=avcodec_encode_video(&c.Ctx, buffer, &buffer_size, f);
  //println("encode ready")
  //println(esize)
  if(esize>0){
    result.avpacket.size=(_Ctype_int)(esize)
    result.avpacket.data=(*_Ctypedef_uint8_t)(unsafe.Pointer(&buffer[0]))
    result.avpacket.stream_index=0
    result.avpacket.duration=0
    result.avpacket.flags=0
    if (c.Ctx.ctx.coded_frame!=nil) {
	if (c.Ctx.ctx.coded_frame.key_frame==1) {
    	    result.avpacket.flags |= 0x0001
        }
        //result.avpacket.pts = c.Ctx.ctx.coded_frame.pts;
    }
    return result
  }
return nil 
}

func(c * Encoder)encodeAudio(f * Frame)*Packet{
    print("ctx.sample_fmt")
    println(c.Ctx.ctx.sample_fmt)
    bpsf:=av_get_bits_per_sample_fmt(c.Ctx.ctx.sample_fmt)
    print("bits per sample ")
    println(bpsf)
    audio_out_size := (4 * 192 * 1024)
    out_buffer:=make([]byte,audio_out_size)
    print("out buffer ")
    println(out_buffer)
    if(c.Ctx.ctx.frame_size>1){
	frame_bytes := int(c.Ctx.ctx.frame_size) * bpsf * int(c.Ctx.ctx.channels);
	print("frame bytes ")
	println(frame_bytes)
    }
    return nil
}
