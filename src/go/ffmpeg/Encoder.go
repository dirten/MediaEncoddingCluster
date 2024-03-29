package gmf
import "math"
import "log"
//import "unsafe"
type Encoder struct {
    Coder
    audio_fifo * AVFifoBuffer
    last_dts int64
    stream_index int
    Track * Track
    buffer_size int
    buffer []byte
}

func(c * Encoder)Open(){
    c.open(CODEC_TYPE_ENCODER)
    c.last_dts=0
    c.audio_fifo=av_fifo_alloc(1024)
    log.Printf("Encoder Oppened")
    c.stream_index=0
    size := c.Ctx.ctx.width * c.Ctx.ctx.height;
    c.buffer_size = int(math.Fmax(float64(4 * 192 * 1024), float64(6 * size + 200)));
    c.buffer/*[]byte*/ =make([]byte,c.buffer_size+8)
}

func(c * Encoder)Encode(f * Frame)*Packet{
  if(int32(c.Ctx.ctx.codec_type)==CODEC_TYPE_VIDEO){
    return c.encodeVideo(f)
  }
  if(int32(c.Ctx.ctx.codec_type)==CODEC_TYPE_AUDIO){
    return c.encodeAudio(f)
  }
  return nil
}


func (c*Encoder)GetTimeBase()Rational{
    return Rational{int(c.Ctx.ctx.time_base.num),int(c.Ctx.ctx.time_base.den)}
}

func (c*Encoder)GetFrameRate()Rational{
    return Rational{int(c.Ctx.ctx.time_base.num),int(c.Ctx.ctx.time_base.den)}
}

func(c * Encoder)encodeVideo(f * Frame)*Packet{
  f.avframe.pts=_Ctypedef_int64_t(f.Pts.Time)
  //  return nil
  //log.Printf("Encoder Frame %s", f.String())
  for i:=0;i<f.frame_count;i++{
  f.avframe.pts=_Ctypedef_int64_t(c.last_dts)
  esize:=avcodec_encode_video(&c.Ctx, c.buffer, &c.buffer_size, f);
  //println("encode ready")
  //println(esize)
  c.last_dts++
  if(esize>0){
    var result Packet//=new(Packet)
    av_init_packet(&result)
    //result.avpacket.size=(_Ctype_int)(esize)
    //result.avpacket.data=(*_Ctypedef_uint8_t)(unsafe.Pointer(&buffer[0]))
    result.Size=esize
    //result.Data=c.buffer
    result.Data=make([]byte,esize+8)
    for i:=0;i<esize;i++{
	result.Data[i]=c.buffer[i]
    }

    result.Stream=c.stream_index
    result.Duration=Timestamp{int64(c.Ctx.ctx.ticks_per_frame),Rational{int(c.Ctx.ctx.time_base.num),int(c.Ctx.ctx.time_base.den)}}
    result.Flags=0
    if (c.Ctx.ctx.coded_frame!=nil) {
	if (c.Ctx.ctx.coded_frame.key_frame==1) {
    	    result.Flags |= 0x0001
        }
        result.Pts=Timestamp{int64(c.Ctx.ctx.coded_frame.pts),Rational{int(c.Ctx.ctx.time_base.num),int(c.Ctx.ctx.time_base.den)}}
        result.Dts=Timestamp{int64(c.last_dts),Rational{int(c.Ctx.ctx.time_base.num),int(c.Ctx.ctx.time_base.den)}}
        //result.avpacket.pts = c.Ctx.ctx.coded_frame.pts;
        //result.avpacket.duration = 1
        if(c.Track!=nil){
            c.Track.WritePacket(&result)
        }
    }
    
    //return result
  }
  }
return nil 
}

func(c * Encoder)encodeAudio(f * Frame)*Packet{
    bpsf:=av_get_bits_per_sample_fmt(c.Ctx.ctx.sample_fmt)/8
    //audio_out_size := (4 * 192 * 1024)
    //out_buffer:=make([]byte,audio_out_size+8)
    //print("frame buffer size ")
    //println(f.size)
    //return nil
    if(c.Ctx.ctx.frame_size>1){
	frame_bytes := int(c.Ctx.ctx.frame_size) * bpsf * int(c.Ctx.ctx.channels);
        if(av_fifo_realloc(c.audio_fifo,uint(av_fifo_size(c.audio_fifo) + f.size))<0){
            log.Printf("Error while realloc audio fifo")
        }
        av_fifo_generic_write(c.audio_fifo, f.buffer, f.size);
        audio_buf_size := (2 * 128 * 1024)
        audio_buf := make([]byte,audio_buf_size+8)//static_cast<uint8_t*> (av_malloc(audio_buf_size));
        //log.Printf("bpsf=%d framesize=%d audiobufsize=%d ctx.frame_size=%d frame_bytes=%d", bpsf,f.size, audio_buf_size,c.Ctx.ctx.frame_size, frame_bytes)
        for (av_fifo_size(c.audio_fifo) >= frame_bytes) {
            av_fifo_generic_read(c.audio_fifo, audio_buf, frame_bytes);
              out_size := avcodec_encode_audio(
              &c.Ctx,
              c.buffer,
              &c.buffer_size,
              audio_buf)
            if (out_size < 0) {
              log.Printf("Error Encoding Audio Frame")
            }
            if (out_size == 0) {
              continue
            }
            
            if (out_size > 0  ) {
              //log.Printf("Encoded Audio Frame size:%d" , out_size);
            }
            var result *Packet=new(Packet)
            av_init_packet(result)
            result.Size=out_size
            result.Data=make([]byte,c.buffer_size+8)
            for i:=0;i<out_size;i++{
        	result.Data[i]=c.buffer[i]
            }
            result.Duration=Timestamp{int64(c.Ctx.ctx.frame_size),Rational{1,int(c.Ctx.ctx.sample_rate)}}
            result.Pts=Timestamp{int64(c.last_dts),Rational{1,int(c.Ctx.ctx.sample_rate)}}
            result.Dts=Timestamp{int64(c.last_dts),Rational{1,int(c.Ctx.ctx.sample_rate)}}
            result.Stream=c.stream_index
            c.last_dts+=result.Duration.Time
            /*special handling for vbr audio encoder*/
            if (c.Ctx.ctx.coded_frame!=nil) {
        	//println("audio coded frame")
        	result.Dts=Timestamp{int64(c.Ctx.ctx.coded_frame.pts),Rational{1,int(c.Ctx.ctx.sample_rate)}}
        	result.Pts=Timestamp{int64(c.Ctx.ctx.coded_frame.pts),Rational{1,int(c.Ctx.ctx.sample_rate)}}
        	//println(result.String())
            }
    	    result.Flags |= 0x0001

            //println(result.String())
            if(c.Track!=nil){
                c.Track.WritePacket(result)
            }
            

        }
    }
    return nil
}
