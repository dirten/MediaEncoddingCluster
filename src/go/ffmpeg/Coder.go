package gmf

import "log"
import "strconv"

type Coder struct {
  Parameter map[string]string
  Ctx CodecContext
  Codec Codec
  Valid bool
  initialized bool
  codec_type int
  Framerate int
  Pixelformat int
}


func (self * Coder)init(){
    if(self.Parameter==nil){
	println("creating Parameter map")
	self.Parameter=make(map[string]string)
    }
    self.initialized=true
}

func (self * Coder)SetParameter(key, val string){
  self.init()
  self.Parameter[key]=val
}

func (self * Coder)GetParameter(key string)string{
  return self.Parameter[key]
}

func (self * Coder)GetParameters()map[string]string{
  return self.Parameter
}


func(c * Coder)open(t int){
  c.codec_type=t
    
  c.init()
  
  if(c.codec_type==CODEC_TYPE_DECODER){
    c.Codec=avcodec_find_decoder(c.Ctx.ctx.codec_id)
  }
  
  if(c.codec_type==CODEC_TYPE_ENCODER){
    log.Printf("first prepare")
    c.prepare()
    c.Codec=avcodec_find_encoder(c.Ctx.ctx.codec_id)
  }
  
  if(c.Codec.codec==nil){
    log.Printf("could not find Codec for id %d",c.Ctx.ctx.codec_id)
    return
  }
  //avcodec_get_context_defaults2(c.Ctx, c.Codec);
    //c.Ctx.ctx.codec_id=c.Codec.codec.codec_id
    //if(c.Ctx.ctx.codec_id<0x10000){
	c.Ctx.ctx.codec_type=c.Codec.codec._type
    //}
    c.Ctx.ctx.pix_fmt=uint32(c.Pixelformat)
    //c.Ctx.ctx.width=320
    //c.Ctx.ctx.height=240
    
//  c.Ctx.ctx.time_base.num=25
//  c.Ctx.ctx.time_base.den=1
  c.setCodecParams()
  res:=avcodec_open(c.Ctx,c.Codec)
  
  if(res<0){
    log.Printf("error openning codec for id %d\n",c.Ctx.ctx.codec_id)
    return
  }
  log.Printf("codec openned for id %d\n",c.Ctx.ctx.codec_id)
  c.Valid=true
}

func(self* Coder)prepare(){
  if(self.Ctx.ctx==nil){
    cid,_:=strconv.Atoui64(self.Parameter["codecid"])
    log.Printf("searching for codec id %d", cid)
    self.Ctx=*avcodec_alloc_context();
    self.Ctx.ctx.codec_id=uint32(cid)
  }
  width,err:=strconv.Atoi(self.Parameter["width"])
  if(err==nil){
    self.Ctx.ctx.width=_Ctype_int(width)
    log.Printf("setting width to %d",self.Ctx.ctx.width)
  }else{
//    println(err.String())
  }
  height,err:=strconv.Atoi(self.Parameter["height"])
  if(err==nil){
    self.Ctx.ctx.height=_Ctype_int(height)
    log.Printf("setting height to %d",self.Ctx.ctx.height)
  }else{
//    println(err.String())
  }
  channels,err:=strconv.Atoi(self.Parameter["channels"])
  if(err==nil){
    self.Ctx.ctx.channels=_Ctype_int(channels)
    log.Printf("setting channels to %d",self.Ctx.ctx.channels)
  }else{
//    println(err.String())
  }

  /**
  * @TODO: settng the fixed params like width, height, channels...
  */
}

func(self*Coder)setCodecParams(){
  for mkey,mval:=range self.Parameter {
    av_set_string(&self.Ctx, mkey, mval);
  }
}
