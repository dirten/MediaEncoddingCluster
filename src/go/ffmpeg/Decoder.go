package gmf



type Decoder struct {
    Coder
}
func(c * Decoder)Open(){
    c.open(CODEC_TYPE_DECODER)
}

func(c * Decoder)Decode(p Packet)*Frame{
    //println(p.avpacket)
  if(c.Ctx.ctx.codec_type==CODEC_TYPE_VIDEO){
    return c.decodeVideo(p)
  }
  if(c.Ctx.ctx.codec_type==CODEC_TYPE_AUDIO){
    return c.decodeAudio(p)
  }
  return nil
}

func(c * Decoder)decodeAudio(p Packet)*Frame{
  if(!c.Valid){
    return nil
  }
  samples_size:= AVCODEC_MAX_AUDIO_FRAME_SIZE;
  outbuf:= make([]byte,samples_size)//(*C.uint8_t) (C.av_malloc(C.uint(samples_size)));
  avcodec_decode_audio(&c.Ctx, &outbuf, &samples_size, &p)
  var frame * Frame=new(Frame)
  frame.audiobuffer=&outbuf
  frame.isFinished=true
  return frame
}

func(c * Decoder)decodeVideo(p Packet)*Frame{
  if(!c.Valid){
    return nil
  }

  height:=int(c.Ctx.ctx.height)
  width:=int(c.Ctx.ctx.width)
  var frame * Frame=new(Frame)
  frame.isFinished=false

  numBytes:= avpicture_get_size(0, width, height)
  if(numBytes>0){
    var buffer []byte=make([]byte,numBytes)
    avpicture_fill(frame, &buffer, 0, width, height);
  }


  var frameFinished int=0
  avcodec_decode_video(&c.Ctx,frame,&frameFinished,&p)
  if(frameFinished>0){
    frame.isFinished=true
    frame.width=int(width)
    frame.height=int(height)
    frame.size=numBytes
    frame.Pts=p.Pts
    frame.Duration=p.Duration
  }
//  println("frame decoder")
//  println(frame.avframe)
  return frame
}
