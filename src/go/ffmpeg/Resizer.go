package gmf

import "log"
type Resizer struct{
    ctx * SwsContext
    width int
    height int
    fmt int
    frame Frame
    buffer *[]byte
}


func (self * Resizer)Init(dec * Decoder, enc * Encoder){
    self.ctx=new(SwsContext)
    sws_scale_getcontext(self.ctx,int(dec.Ctx.ctx.width), int(dec.Ctx.ctx.height), int(dec.Ctx.ctx.pix_fmt), int(enc.Ctx.ctx.width), int(enc.Ctx.ctx.height), int(enc.Ctx.ctx.pix_fmt), 1)
    self.width=int(enc.Ctx.ctx.width)
    self.height=int(enc.Ctx.ctx.height)
    self.fmt=int(enc.Ctx.ctx.pix_fmt)
    log.Printf("setting swscale from %d/%d:%d to %d/%d:%d",int(dec.Ctx.ctx.width),int(dec.Ctx.ctx.height),int(dec.Ctx.ctx.pix_fmt),int(enc.Ctx.ctx.width),int(enc.Ctx.ctx.height),int(enc.Ctx.ctx.pix_fmt))
    self.frame = Frame{}
    /*
    numBytes:= avpicture_get_size(uint32(self.fmt), self.width, self.height)
    if(numBytes>0){
	self.buffer=av_malloc(numBytes)
	avpicture_fill(&self.frame, self.buffer, 0, self.width, self.height);
    }*/
//func sws_scale_getcontext(ctx * SwsContext, srcwidth, srcheight, srcfmt, trgwidth,trgheight,trgfmt,flags int){

}

func(self*Resizer)Resize(in* Frame)*Frame{
    
    /*avcodec_get_frame_defaults(&self.frame)
    if(avpicture_alloc(&self.frame, self.fmt, self.width, self.height)!=0){
	log.Printf("can not allocate tmp picture, check pixel format")
    }
    */
    
    numBytes:= avpicture_get_size(uint32(self.fmt), self.width, self.height)
    if(numBytes>0){
	self.buffer =av_malloc(numBytes)//make([]byte,numBytes)
	//log.Printf("memory address %i", &self.buffer)
	avpicture_fill(&self.frame, self.buffer, 0, self.width, self.height);
    }
    
    if(sws_scale(self.ctx, in, &self.frame)==0){
	log.Printf("failed to resize the image")
    }
    self.frame.avframe.pts=in.avframe.pts
    return & self.frame
}

