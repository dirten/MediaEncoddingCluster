package gmf
import "log"
type Resampler struct{
    ctx * ResampleContext
    isize int
    osize int
    outbuffer []byte
    channels int
    sample_rate int
    dec * Decoder
    enc * Encoder
}

func(self*Resampler)Init(dec * Decoder, enc * Encoder){
    self.ctx=new(ResampleContext)
    self.ctx=av_audio_resample_init(
            int(enc.Ctx.ctx.channels),
            int(dec.Ctx.ctx.request_channels),
            int(enc.Ctx.ctx.sample_rate),
            int(dec.Ctx.ctx.sample_rate),
            int(enc.Ctx.ctx.sample_fmt),
            int(dec.Ctx.ctx.sample_fmt))
            if(self.ctx.ctx==nil){
                println("Could not create resample context!!!!!!!!!!!!!!")
            }
    self.isize=av_get_bits_per_sample_fmt(dec.Ctx.ctx.sample_fmt)/8
    self.osize=av_get_bits_per_sample_fmt(enc.Ctx.ctx.sample_fmt)/8
    self.outbuffer=make([]byte,(2 * 128 * 1024))
    self.channels=int(enc.Ctx.ctx.channels)
    self.sample_rate=int(enc.Ctx.ctx.sample_rate)
    self.dec=dec
    self.enc=enc
    log.Printf("ResampleContext inch:%d;outch:%d;inrate:%d;outrate:%d;infmt:%d,outfmt:%d",int(dec.Ctx.ctx.request_channels),int(enc.Ctx.ctx.channels),int(dec.Ctx.ctx.sample_rate),int(enc.Ctx.ctx.sample_rate),int(dec.Ctx.ctx.sample_fmt),int(enc.Ctx.ctx.sample_fmt))
}

func(self*Resampler)Resample(f * Frame)*Frame{
    frame:=new(Frame)
    out_size := audio_resample(self.ctx, self.outbuffer, f.buffer, (f.size / (self.channels * self.isize)))
    frame.buffer=self.outbuffer
    frame.size=out_size*self.channels*self.osize
    frame.Duration=Timestamp{int64(out_size),Rational{1,self.sample_rate}}

    
    last_insamples:=av_rescale_q(int64(f.size/self.isize),Rational{1,int(self.dec.Ctx.ctx.sample_rate)},Rational{1,int(self.enc.Ctx.ctx.sample_rate)})
    last_outsamples:=int64(frame.size/self.osize)
    delta:=av_clip(int(last_insamples-last_outsamples),-2,2)
    //log.Printf("Resample Compensate delta = %d insamples = %d outsamples = %d", delta, last_insamples, last_outsamples)
    av_resample_compensate(self.ctx, delta,int(self.enc.Ctx.ctx.sample_rate/2))

    return frame
}