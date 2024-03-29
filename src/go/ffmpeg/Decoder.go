package gmf

import "log"
import "unsafe"
import "fmt"
type Decoder struct {
    Coder
    pts int64
    frame_rate Rational
    time_base Rational
}

func(c * Decoder)Open(){
    c.Coder.open(CODEC_TYPE_DECODER)
    c.pts=0
}
func (self * Decoder)SetParameter(key, val string){
    if(key=="frame_rate"){
	fmt.Sscanf(val,"%d/%d",&self.frame_rate.Num,&self.frame_rate.Den)
    }else{
        self.Coder.SetParameter(key,val)
    }
}

func (self * Decoder)GetParameters()map[string]string{
    self.Coder.GetParameters()
    self.Parameter["frame_rate"]=fmt.Sprintf("%d/%d",self.frame_rate.Num,self.frame_rate.Den)
    return self.Parameter
}

func(c * Decoder)Decode(p * Packet)*Frame{
  p2:=new(AVPacket)
  av_init_packet2(p2)
  p2.size=(_Ctype_int)(p.Size)
  p2.data=(*_Ctypedef_uint8_t)(unsafe.Pointer(&p.Data[0]))
  p2.pts=(_Ctypedef_int64_t)(p.Pts.Time)
  p2.duration=(_Ctype_int)(p.Duration.Time)
  p2.flags=_Ctype_int(p.Flags)
  p2.stream_index=_Ctype_int(p.Stream)
  p2.dts=_Ctypedef_int64_t(AV_NOPTS_VALUE)

  if(int32(c.Ctx.ctx.codec_type)==CODEC_TYPE_VIDEO){
    return c.decodeVideo(p2)
  }
  if(int32(c.Ctx.ctx.codec_type)==CODEC_TYPE_AUDIO){
    return c.decodeAudio(p2)
  }
  return nil
}

func(c * Decoder)decodeAudio(p * AVPacket)*Frame{
  if(!c.Valid){
    return nil
  }
  //println("decoder audio")
  samples_size:= AVCODEC_MAX_AUDIO_FRAME_SIZE;
  outbuf:= make([]byte,samples_size)//(*C.uint8_t) (C.av_malloc(C.uint(samples_size)));
  avcodec_decode_audio(&c.Ctx, outbuf, &samples_size, p)
  var frame * Frame=new(Frame)
  frame.buffer=outbuf
  frame.size=samples_size
  if(samples_size>0){
    frame.isFinished=true
  }else{
    frame.isFinished=false
    log.Printf("frame not finished")
  }
  return frame
}

func (c*Decoder)GetCodecType()int32{
    return int32(c.Ctx.ctx.codec_type)
}

func (c*Decoder)GetCodecId()int32{
    return int32(c.Ctx.ctx.codec_id)
}

func (c*Decoder)GetTimeBase()Rational{
    return Rational{int(c.Ctx.ctx.time_base.num),int(c.Ctx.ctx.time_base.den)}
}

func (c*Decoder)GetFrameRate()Rational{
    if(c.frame_rate.Den==0&&c.frame_rate.Num==0){
	c.frame_rate.Den=int(c.Ctx.ctx.time_base.num)
	c.frame_rate.Num=int(c.Ctx.ctx.time_base.den)
    }
    return Rational{c.frame_rate.Den,c.frame_rate.Num}
}

func(c * Decoder)decodeVideo(p * AVPacket)*Frame{
  if(!c.Valid){
    return nil
  }

  height:=int(c.Ctx.ctx.height)
  width:=int(c.Ctx.ctx.width)
  var frame * Frame=NewFrame(int(c.Ctx.ctx.pix_fmt), int(c.Ctx.ctx.width), int(c.Ctx.ctx.height))

  var frameFinished int=0
  //println(p.avpacket.data)
  avcodec_decode_video(&c.Ctx,frame,&frameFinished,p)
  if(frameFinished>0){
    frame.isFinished=true
    frame.width=int(width)
    frame.height=int(height)
    //frame.size=numBytes
    c.pts+=int64(c.frame_rate.Den)
    frame.Pts=Timestamp{c.pts,Rational{c.frame_rate.Den,c.frame_rate.Num}}
    frame.Duration=Timestamp{int64(c.frame_rate.Den),Rational{c.frame_rate.Den,c.frame_rate.Num}}
  }
//  println("frame decoder")
//  println(frame.avframe)
  return frame
}
