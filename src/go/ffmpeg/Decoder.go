package gmf

import "log"

type Decoder struct {
    Coder
    pts int64
    frame_rate Rational
    time_base Rational
}

func(c * Decoder)Open(){
    c.open(CODEC_TYPE_DECODER)
    c.pts=0
}

func(c * Decoder)Decode(p * Packet)*Frame{
  //println(p.avpacket)
  if(c.Ctx.ctx.codec_type==CODEC_TYPE_VIDEO){
    return c.decodeVideo(p)
  }
  if(c.Ctx.ctx.codec_type==CODEC_TYPE_AUDIO){
    return c.decodeAudio(p)
  }
  return nil
}

func(c * Decoder)decodeAudio(p * Packet)*Frame{
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
    return c.Ctx.ctx.codec_type
}

func (c*Decoder)GetCodecId()int32{
    return c.Ctx.ctx.codec_id
}

func (c*Decoder)GetTimeBase()Rational{
    return Rational{int(c.Ctx.ctx.time_base.num),int(c.Ctx.ctx.time_base.den)}
}

func (c*Decoder)GetFrameRate()Rational{
    return Rational{c.frame_rate.Den,c.frame_rate.Num}
}

func(c * Decoder)decodeVideo(p * Packet)*Frame{
  if(!c.Valid){
    return nil
  }

  height:=int(c.Ctx.ctx.height)
  width:=int(c.Ctx.ctx.width)
  var frame * Frame=NewFrame(int(c.Ctx.ctx.pix_fmt), int(c.Ctx.ctx.width), int(c.Ctx.ctx.height))

  var frameFinished int=0
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
